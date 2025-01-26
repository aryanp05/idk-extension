import sqlite3
from pathlib import Path

DB_PATH = Path("tokens.db")
SCHEMA = Path(__file__).parent / "schema.sql"


def main():
    with sqlite3.connect(DB_PATH) as cx, open(SCHEMA) as fp:
        cx.executescript(fp.read())
    print("Database initialised at", DB_PATH)


if __name__ == "__main__":
    main() 