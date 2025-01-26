from fastapi import FastAPI
import sqlite3, os
import pinecone
from llm_utils import embed, call_llm

DB="tokens.db"
INDEX_NAME="infermath"
pinecone.init(api_key=os.getenv("PINECONE_API_KEY"), environment=os.getenv("PINECONE_ENV","us-west1-gcp"))
index = pinecone.Index(INDEX_NAME)

app = FastAPI(title="InferMath API")


@app.get("/health")
def health():
    return {"status": "ok"}


@app.post("/answer")
def answer(q: str):
    q_vec = embed(q)
    hits = index.query(vector=q_vec, top_k=3, include_metadata=True)
    pages = [int(m.id) for m in hits.matches]

    # fetch the tokens for each page
    conn = sqlite3.connect(DB)
    cur = conn.cursor()
    snippets = []
    for pid in pages:
        rows = cur.execute(f"SELECT lexeme FROM page_{pid}").fetchall()
        snippets.append(" ".join(r[0] for r in rows))
    conn.close()

    context = "\n".join(snippets)
    prompt = f"Answer the question using the following notes context.\n\nContext:\n{context}\n\nQuestion: {q}\nAnswer:"
    answer = call_llm(prompt)
    return {"answer": answer, "pages": pages} 