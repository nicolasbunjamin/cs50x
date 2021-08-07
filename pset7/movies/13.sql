SELECT DISTINCT people.name FROM people
JOIN stars ON people.id = stars.person_id

WHERE stars.movie_id IN

(SELECT  stars.movie_id FROM people 
JOIN stars ON people.id = stars.person_id
JOIN movies ON stars.movie_id = movies.id
WHERE people.name = "Kevin Bacon"
AND people.birth = 1958)

AND people.name != "Kevin Bacon";
