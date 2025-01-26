import React, { useState } from 'react';
import { uploadLatexFile, askQuestion } from './api';

const App: React.FC = () => {
  const [file, setFile] = useState<File | null>(null);
  const [question, setQuestion] = useState('');
  const [answer, setAnswer] = useState('');
  const [status, setStatus] = useState<'idle' | 'uploading' | 'ready' | 'asking'>('idle');

  const handleFileChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    const f = e.target.files?.[0];
    if (f) setFile(f);
  };

  const handleUpload = async () => {
    if (!file) return;
    setStatus('uploading');
    try {
      await uploadLatexFile(file);
      setStatus('ready');
    } catch (e) {
      console.error(e);
      setStatus('idle');
    }
  };

  const handleAsk = async () => {
    if (!question.trim()) return;
    setStatus('asking');
    try {
      const res = await askQuestion(question);
      setAnswer(res);
    } catch (e) {
      console.error(e);
      setAnswer('[error fetching answer]');
    }
    setStatus('ready');
  };

  return (
    <div className="container">
      <header>
        <h1>InferMath</h1>
      </header>

      <section className="upload">
        <h2>1. Upload LaTeX Notes</h2>
        <input type="file" accept=".tex" onChange={handleFileChange} />
        <button disabled={!file || status === 'uploading'} onClick={handleUpload}>
          {status === 'uploading' ? 'Uploading…' : 'Upload'}
        </button>
      </section>

      <section className="qa">
        <h2>2. Ask a Question</h2>
        <input
          type="text"
          placeholder="Type your question…"
          value={question}
          onChange={(e) => setQuestion(e.target.value)}
        />
        <button disabled={status !== 'ready'} onClick={handleAsk}>
          Ask
        </button>

        {answer && (
          <div className="answer">
            <h3>Answer</h3>
            <p>{answer}</p>
          </div>
        )}
      </section>
    </div>
  );
};

export default App; 