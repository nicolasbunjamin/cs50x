SELECT title, rating FROM movies JOIN ratings ON id = movie_id
WHERE rating != "//N" AND year = 2010
ORDER BY rating DESC , title ASC;