import axios from 'axios';

export async function uploadLatexFile(file: File): Promise<void> {
  const formData = new FormData();
  formData.append('file', file);
  await axios.post('/upload', formData, {
    headers: { 'Content-Type': 'multipart/form-data' }
  });
}

export async function askQuestion(question: string): Promise<string> {
  const { data } = await axios.post('/answer', { q: question });
  return data.answer ?? '[no answer]';
} 