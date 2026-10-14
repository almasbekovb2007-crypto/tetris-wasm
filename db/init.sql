-- Create table for application settings
CREATE TABLE IF NOT EXISTS settings (
  id SERIAL PRIMARY KEY,

  sound_enabled BOOLEAN DEFAULT TRUE,
  music_enabled BOOLEAN DEFAULT TRUE,
  theme_collor BOOLEAN DEFAULT TRUE,

  field_width INTEGER DEFAULT 10,
  field_height INTEGER DEFAULT 20,
  piece_size INTEGER DEFAULT 4,

  key_left INTEGER DEFAULT 16777234,
  key_right INTEGER DEFAULT 16777236,
  key_down INTEGER DEFAULT 16777237,
  key_rotate INTEGER DEFAULT 16777235,
  key_drop INTEGER DEFAULT 32,
  key_pause INTEGER DEFAULT 80,

  updated_at TIMESTAMP WITHOUT TIME ZONE DEFAULT NOW()
);
CREATE TABLE IF NOT EXISTS game_sessions (
  id SERIAL PRIMARY KEY,

  score INTEGER DEFAULT 0,
  grid TEXT,

  settings_id INTEGER,
  CONSTRAINT game_sessions_settings_id_fkey
    FOREIGN KEY (settings_id)
    REFERENCES settings(id)
    ON DELETE SET NULL,

  created_at TIMESTAMP WITHOUT TIME ZONE DEFAULT NOW()
);
CREATE TABLE IF NOT EXISTS records (
  id SERIAL PRIMARY KEY,

  session_id INTEGER,
  CONSTRAINT records_session_id_fkey
    FOREIGN KEY (session_id)
    REFERENCES game_sessions(id)
    ON DELETE CASCADE,

  played_at TIMESTAMP WITHOUT TIME ZONE DEFAULT NOW()
);