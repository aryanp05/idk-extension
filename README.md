# InferMath – Hackathon Prototype


InferMath converts LaTeX course notes into searchable token vectors and uses an LLM to answer questions about the notes.

Directory overview
------------------
```
scanner/      C++ lexer → tokens + page numbers

db/           SQLite schema + migration helper
ingest/       Python script to embed each page and push to a vector DB (Pinecone)
api/          FastAPI micro-service that answers questions with LLM + context
```

Quick start (demo mode)
-----------------------
```bash
# build the lexer (requires sqlite3 + CMake)
cd scanner && mkdir build && cd build && cmake .. && make && cd ../../

# scan a toy LaTeX file (one token per line for demo simplicity)
cat examples/demo.tex | ./scanner/build/latex_scanner tokens.db

# initialise DB schema
python db/migrate.py

# vectorise the pages (requires OPENAI_API_KEY or offline SBERT)
python ingest/vectorise.py

# run the API (needs PINECONE_API_KEY)
uvicorn api.query_service:app --reload



BTW MATTHEW SUCKS THAT GUY IS SUCH A DUMB CODER HE NEEDS TO GET GOOD AT CODING BRO GOSH HOLY
