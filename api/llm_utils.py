import os
from sentence_transformers import SentenceTransformer
import openai

_MODEL = None


def _ensure_model():
    global _MODEL
    if _MODEL is None:
        _MODEL = SentenceTransformer("all-MiniLM-L6-v2")
    return _MODEL


def embed(text: str):
    """Return embedding vector for text using SBERT."""
    return _ensure_model().encode(text).tolist()


def call_llm(prompt: str):
    """Call OpenAI ChatCompletion with a safety fallback."""
    api_key = os.getenv("OPENAI_API_KEY")
    if not api_key:
        return "[LLM response placeholder – OPENAI_API_KEY not set]"

    openai.api_key = api_key
    response = openai.ChatCompletion.create(
        model="gpt-3.5-turbo",
        messages=[{"role": "user", "content": prompt}],
        temperature=0.2,
    )
    return response.choices[0].message.content.strip() 