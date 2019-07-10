CREATE TABLE convs(id INTEGER PRIMARY KEY, conv_name TEXT UNIQUE NOT NULL, conv_description TEXT NOT NULL);

CREATE TABLE spots(id INTEGER PRIMARY KEY, conv_id INTEGER, spot_date TEXT NOT NULL, val REAL NOT NULL, FOREIGN KEY(conv_id) REFERENCES convs(id) UNIQUE(conv_id, spot_date));

CREATE TABLE version(version INTEGER);
INSERT INTO version(version) VALUES(1);
