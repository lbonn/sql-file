SAVEPOINT MIGRATION;

CREATE TABLE convs(id INTEGER PRIMARY KEY, conv_name TEXT UNIQUE NOT NULL, conv_description TEXT NOT NULL);
INSERT INTO convs(conv_name, conv_description) VALUES("EURUSD", "euros / dollars");

CREATE TABLE spots(id INTEGER PRIMARY KEY, conv_id INTEGER, spot_date TEXT NOT NULL, val REAL NOT NULL, FOREIGN KEY(conv_id) REFERENCES convs(id) UNIQUE(conv_id, spot_date));

INSERT INTO spots(conv_id, spot_date, val) SELECT convs.id, eurusd.spot_date, eurusd.val FROM convs INNER JOIN eurusd ON convs.conv_name = "EURUSD";

DROP TABLE eurusd;

UPDATE version SET version = 1;

RELEASE MIGRATION;
