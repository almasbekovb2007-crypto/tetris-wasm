require('dotenv').config();
const express = require('express');
const { Pool } = require('pg');
const path = require('path');

const app = express();
const port = process.env.PORT || 3000;
const pool = new Pool({ connectionString: process.env.DATABASE_URL });

app.use(express.json());
app.use(express.static(path.resolve(__dirname)));

app.get('/api/ping', async (req, res) => {
  res.json({ status: 'ok' });
});

app.get('/api/records', async (req, res) => {
  try {
    const result = await pool.query('SELECT grid, score, created_at FROM records ORDER BY score DESC, created_at DESC');
    res.json(result.rows);
  } catch (err) {
    console.error(err);
    res.status(500).json({ error: 'Unable to load records' });
  }
});

app.get('/api/record', async (req, res) => {
  const grid = req.query.grid;
  if (!grid) return res.status(400).json({ error: 'grid is required' });
  try {
    const result = await pool.query('SELECT grid, score FROM records WHERE grid = $1', [grid]);
    if (result.rows.length === 0) return res.json({ grid, score: 0 });
    res.json(result.rows[0]);
  } catch (err) {
    console.error(err);
    res.status(500).json({ error: 'Unable to load record' });
  }
});

app.post('/api/record', async (req, res) => {
  const { grid, score } = req.body;
  if (!grid || typeof score !== 'number') return res.status(400).json({ error: 'grid and score are required' });
  try {
    await pool.query(
      'INSERT INTO records(grid, score) VALUES ($1, $2) ON CONFLICT (grid) DO UPDATE SET score = GREATEST(records.score, EXCLUDED.score), created_at = NOW()',
      [grid, score]
    );
    res.json({ success: true });
  } catch (err) {
    console.error(err);
    res.status(500).json({ error: 'Unable to save record' });
  }
});

app.get('/api/settings', async (req, res) => {
  try {
    const result = await pool.query("SELECT value FROM settings WHERE name = 'default' LIMIT 1");
    if (result.rows.length === 0) return res.json({});
    res.json(result.rows[0].value);
  } catch (err) {
    console.error(err);
    res.status(500).json({ error: 'Unable to load settings' });
  }
});

app.post('/api/settings', async (req, res) => {
  const value = req.body;
  if (!value || typeof value !== 'object') return res.status(400).json({ error: 'Invalid settings payload' });
  try {
    await pool.query(
      `INSERT INTO settings(name, value) VALUES ('default', $1)
       ON CONFLICT (name) DO UPDATE SET value = EXCLUDED.value, updated_at = NOW()`,
      [value]
    );
    res.json({ success: true });
  } catch (err) {
    console.error(err);
    res.status(500).json({ error: 'Unable to save settings' });
  }
});

app.listen(port, () => {
  console.log(`Server listening on http://localhost:${port}`);
});
