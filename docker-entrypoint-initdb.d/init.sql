CREATE TABLE App_user
(
  Id SERIAL PRIMARY KEY,
  Username VARCHAR(64) NOT NULL,
  Password VARCHAR(64) NOT NULL,
  Gender VARCHAR(8) NOT NULL
);

CREATE TABLE Set
(
  Id SERIAL PRIMARY KEY,
  Name VARCHAR(64) NOT NULL,
  Creation_date DATE,
  Creator_id INT REFERENCES App_user(Id)
);

CREATE TABLE Flashcard
(
  Id SERIAL PRIMARY KEY,
  Question VARCHAR(64) NOT NULL,
  Answer VARCHAR(64) NOT NULL,
  Set_id INT NOT NULL REFERENCES Set(Id)
);

CREATE TABLE Users_sets
(
  Id SERIAL PRIMARY KEY,
  Rate INT,
  User_id INT NOT NULL REFERENCES App_user(Id),
  Set_id INT NOT NULL REFERENCES Set(Id)
);

CREATE TABLE User_flashcard
(
  Importance INT NOT NULL,
  Flashcard_id INT NOT NULL REFERENCES Flashcard(Id),
  Users_sets_id INT NOT NULL REFERENCES Users_sets(Id),
  PRIMARY KEY (Flashcard_id, Users_sets_id)
);