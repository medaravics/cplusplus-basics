#!/usr/bin/env python3
import re
import argparse
import subprocess
import sys
from pathlib import Path


def migrate_code(code: str) -> str:
    # -------------------------------------------------
    # 1. Remove dbConnObj NULL check block
    # -------------------------------------------------
    code = re.sub(
        r'if\s*\(\s*dbConnObj\s*==\s*NULL\s*\)\s*\{.*?return\s+CODE_EXCEPTION\s*;\s*\}',
        '',
        code,
        flags=re.DOTALL
    )

    # -------------------------------------------------
    # 2. Remove m_RunTimeCntxt declaration and assignment
    # -------------------------------------------------
    code = re.sub(
        r'DbConnectionObject\s*\*\s*m_RunTimeCntxt\s*=\s*NULL\s*;',
        '',
        code
    )

    code = re.sub(
        r'm_RunTimeCntxt\s*=\s*\(DbConnectionObject\*\)\s*dbConnObj\s*;',
        '',
        code
    )

    # -------------------------------------------------
    # 3. Remove m_RunTimeCntxt NULL check block
    # -------------------------------------------------
    code = re.sub(
        r'if\s*\(\s*m_RunTimeCntxt\s*==\s*NULL\s*\)\s*\{.*?return\s+CODE_EXCEPTION\s*;\s*\}',
        '',
        code,
        flags=re.DOTALL
    )

    # -------------------------------------------------
    # 4. Remove DECLARE SECTION
    # -------------------------------------------------
    code = re.sub(
        r'EXEC\s+SQL\s+BEGIN\s+DECLARE\s+SECTION\s*;.*?EXEC\s+SQL\s+END\s+DECLARE\s+SECTION\s*;',
        '',
        code,
        flags=re.DOTALL
    )

    # -------------------------------------------------
    # 5. Convert EXEC SQL queries
    # -------------------------------------------------
    def replace_sql(match):
        sql_body = match.group(0)
        sql = re.sub(r'^EXEC\s+SQL', '', sql_body, flags=re.IGNORECASE)
        sql = sql.strip().rstrip(';')

        sql = ' '.join(line.strip() for line in sql.splitlines())

        return (
            f'string query = " {sql} ;";\n'
            f'int recordCount = DbUtil::getAValue(query);'
        )

    code = re.sub(
        r'EXEC\s+SQL\s+(SELECT|INSERT|UPDATE|DELETE).*?;',
        replace_sql,
        code,
        flags=re.DOTALL | re.IGNORECASE
    )

    # -------------------------------------------------
    # 6. Remove remaining EXEC SQL statements
    # -------------------------------------------------
    code = re.sub(
        r'^\s*EXEC\s+SQL.*?;\s*$',
        '',
        code,
        flags=re.MULTILINE
    )

    return code


def git_commit(file_path: Path, message: str):
    try:
        subprocess.run(["git", "add", str(file_path)], check=True)
        subprocess.run(["git", "commit", "-m", message], check=True)
    except subprocess.CalledProcessError:
        print("⚠️ Git commit failed. Is this a git repository?")
        sys.exit(1)


def main():
    parser = argparse.ArgumentParser(
        description="Migrate Pro*C (.pc) file to PostgreSQL utility based C++ code"
    )
    parser.add_argument("--input", required=True, help="Input .pc file")
    parser.add_argument("--output", required=True, help="Output .cpp file")
    parser.add_argument("--commit", help="Git commit message (optional)")

    args = parser.parse_args()

    input_path = Path(args.input)
    output_path = Path(args.output)

    if not input_path.exists():
        print(f"❌ Input file not found: {input_path}")
        sys.exit(1)

    code = input_path.read_text()
    migrated_code = migrate_code(code)
    output_path.write_text(migrated_code)

    print(f"✅ Migration completed: {output_path}")

    if args.commit:
        git_commit(output_path, args.commit)
        print("✅ Changes committed to git")


if __name__ == "__main__":
    main()



