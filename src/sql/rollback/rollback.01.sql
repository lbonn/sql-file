SAVEPOINT ROLLBACK_MIGRATION;

CREATE TABLE eurusd(spot_date TEXT PRIMARY KEY, val REAL NOT NULL);

INSERT INTO eurusd(spot_date, val) SELECT spots.spot_date, spots.val FROM spots INNER JOIN convs WHERE spots.conv_id = convs.id AND convs.conv_name = "EURUSD";

DROP TABLE convs;
DROP TABLE spots;

UPDATE version SET version = 0;

RELEASE ROLLBACK_MIGRATION;
