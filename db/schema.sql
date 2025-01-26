-- master pages table
CREATE TABLE IF NOT EXISTS pages (
  page_id    INTEGER PRIMARY KEY,
  page_name  TEXT,
  created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- individual page tables are created at runtime, e.g.
--   CREATE TABLE page_1 (tok_id INTEGER PRIMARY KEY, type TEXT, lexeme TEXT); 