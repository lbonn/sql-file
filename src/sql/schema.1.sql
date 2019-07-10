CREATE TABLE eurusd(spot_date TEXT PRIMARY KEY, val REAL NOT NULL);

CREATE TABLE rollback_migrations(version_from INTEGER PRIMARY KEY, migration TEXT NOT NULL);

CREATE TABLE version(version INTEGER);
INSERT INTO version(version) VALUES(0);
