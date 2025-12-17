import re
import sys
import os


def migrate_proc_to_pg(input_file, output_file):
    # ---- Validate input file ----
    if not os.path.isfile(input_file):
        print(f"ERROR: Input file does not exist -> {input_file}")
        sys.exit(1)

    # ---- Read input file ----
    with open(input_file, "r", encoding="utf-8", errors="ignore") as f:
        code = f.read()

    # -------------------------------------------------
    # Remove dbConnObj NULL check
    # -------------------------------------------------
    code = re.sub(
        r'if\s*\(\s*dbConnObj\s*==\s*NULL\s*\)\s*\{.*?return\s+CODE_EXCEPTION\s*;\s*\}',
        '',
        code,
        flags=re.DOTALL
    )

    # -------------------------------------------------
    # Remove m_RunTimeCntxt declaration & assignment
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
    # Remove m_RunTimeCntxt NULL check
    # -------------------------------------------------
    code = re.sub(
        r'if\s*\(\s*m_RunTimeCntxt\s*==\s*NULL\s*\)\s*\{.*?return\s+CODE_EXCEPTION\s*;\s*\}',
        '',
        code,
        flags=re.DOTALL
    )

    # -------------------------------------------------
    # Remove DECLARE SECTION
    # -------------------------------------------------
    code = re.sub(
        r'EXEC\s+SQL\s+BEGIN\s+DECLARE\s+SECTION\s*;.*?EXEC\s+SQL\s+END\s+DECLARE\s+SECTION\s*;',
        '',
        code,
        flags=re.DOTALL
    )

    # -------------------------------------------------
    # Convert SELECT / INSERT / UPDATE / DELETE
    # -------------------------------------------------
    def convert_sql(match):
        sql = match.group(0)

        sql = re.sub(r'^EXEC\s+SQL\s+', '', sql, flags=re.IGNORECASE)
        sql = sql.rstrip(';').strip()
        sql = ' '.join(line.strip() for line in sql.splitlines())

        return (
            '\nstring query = " ' + sql + ' ";\n'
            'int recordCount = DbUtil::getAValue(query);\n'
        )

    code = re.sub(
        r'EXEC\s+SQL\s+(SELECT|INSERT|UPDATE|DELETE).*?;',
        convert_sql,
        code,
        flags=re.DOTALL | re.IGNORECASE
    )

    # -------------------------------------------------
    # Remove remaining EXEC SQL lines
    # -------------------------------------------------
    code = re.sub(
        r'^\s*EXEC\s+SQL.*?;\s*$',
        '',
        code,
        flags=re.MULTILINE
    )

    # ---- Write output file ----
    with open(output_file, "w", encoding="utf-8") as f:
        f.write(code)

    print("Migration completed successfully")
    print("Input File :", input_file)
    print("Output File:", output_file)


# -------------------------------------------------
# MAIN: this is what reads input/output arguments
# -------------------------------------------------
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage:")
        print("  python proc_to_pg.py <input_file> <output_file>")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    migrate_proc_to_pg(input_file, output_file)


