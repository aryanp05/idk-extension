import os
from dataclasses import dataclass

@dataclass
class Settings:

    openai_api_key: str = os.getenv("OPENAI_API_KEY", "")
    pinecone_api_key: str = os.getenv("PINECONE_API_KEY", "")
    pinecone_env: str = os.getenv("PINECONE_ENV", "us-west1-gcp")


settings = Settings() 