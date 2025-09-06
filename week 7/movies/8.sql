SELECT name FROM people
JOIN movies ON movie_id = movies.id JOIN stars ON people.id = person_id
WHERE movies.title = "Toy Story"
