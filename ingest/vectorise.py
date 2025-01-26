import sqlite3
import os
from sentence_transformers import SentenceTransformer
import pinecone

DB="tokens.db"
INDEX_NAME="infermath"
MODEL_NAME="all-MiniLM-L6-v2"

model = SentenceTransformer(MODEL_NAME)
pinecone.init(api_key=os.getenv("PINECONE_API_KEY"), environment=os.getenv("PINECONE_ENV","us-west1-gcp"))
index = pinecone.Index(INDEX_NAME)


def main():
    with sqlite3.connect(DB) as cx:
        pages = cx.execute("SELECT page_id FROM pages").fetchall()
        for (pid,) in pages:
            rows = cx.execute(f"SELECT lexeme FROM page_{pid}").fetchall()
            text = " ".join(r[0] for r in rows)
            vec = model.encode(text).tolist()
            index.upsert([(str(pid), vec, {"page_id": pid})])
            print(f"Indexed page {pid}")


if __name__ == "__main__":
    main() 