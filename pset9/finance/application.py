import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Access database
    rows = db.execute("""SELECT symbol, SUM(shares) as total FROM transactions
                    WHERE user_id=:user_id GROUP BY symbol
                    HAVING total > 0""", user_id=session['user_id'])
    cash = db.execute("SELECT cash FROM users WHERE id=:id", id=session['user_id'])
    cash = cash[0]['cash']

    # Organize data
    grand_total = cash
    portfolios = []
    for row in rows:
        ticker = lookup(row['symbol'])
        value = ticker['price'] * row['total']
        grand_total += value
        portfolios.append({
            "symbol": ticker['symbol'],
            "name": ticker['name'],
            "shares": row['total'],
            "price": usd(ticker['price']),
            "value": usd(value)
        })

    return render_template("index.html", portfolios=portfolios, cash=usd(cash), grand_total=usd(grand_total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 403)

        # Ensure shares was submitted
        elif not request.form.get("shares").isdigit():
            return apology("invalid quantity of shares", 400)

        # Look up stock symbol
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        ticker = lookup(symbol)
        if ticker is None:
            return apology("no such stock found", 400)

        # Access cash data
        rows = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
        cash = rows[0]['cash']

        # Check if cash balance is sufficient for purchase
        amount = shares * ticker['price']
        remaining_cash = cash - amount
        if remaining_cash < 0:
            return apology("cash insufficient", 403)

        # Record transaction
        db.execute("""INSERT INTO transactions (user_id, symbol, name, shares, price, amount)
                    VALUES (:user_id, :symbol, :name, :shares, :price, :amount)""", user_id=session["user_id"], symbol=ticker['symbol'], name=ticker['name'], shares=shares, price=ticker['price'], amount=-amount)

        # Update user's cash
        db.execute("UPDATE users SET cash=:remaining_cash WHERE id=:id", remaining_cash=remaining_cash, id=session["user_id"])

        # Redirect user to home page
        flash("Purchase done successfully!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Access database
    rows = db.execute("SELECT * FROM transactions WHERE user_id=:user_id", user_id=session["user_id"])

    # Organize data
    transactions = []
    for row in rows:
        transactions.append({
            "symbol": row['symbol'],
            "name": row['name'],
            "shares": row['shares'],
            "price": usd(row['price']),
            "amount": usd(row['amount']),
            "time": row['time']})

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]['id']

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Look up stock symbol
        symbol = request.form.get("symbol")
        ticker = lookup(symbol)
        if ticker is None:
            return apology("no such stock found", 400)
        return render_template("quoted.html", stock={
            'name': ticker['name'],
            'symbol': ticker['symbol'],
            'price': usd(ticker['price'])
        })

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password confirmation matched password
        elif request.form.get("confirmation") != request.form.get("password"):
            return apology("must provide matching password confirmation", 400)

        # Insert new user data to database
        try:
            primary_key = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"),
                                     hash=generate_password_hash(request.form.get("password")))
        except:
            return apology("username already used", 400)

        # Remember which user has logged in
        session["user_id"] = primary_key

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # MINOR BUGS
    # :( sell handles invalid number of shares
    # expected status code 400, but got 200

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 403)

        # Ensure shares was submitted
        elif not request.form.get("shares").isdigit():
            return apology("invalid number of shares", 400)

        # Look up stock symbol
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        ticker = lookup(symbol)
        if ticker is None:
            return apology("no such stock found", 400)

        # Check if ownership is sufficient for sale
        ownership = db.execute("""SELECT SUM(shares) as shares FROM transactions
                        WHERE user_id=:user_id AND symbol=:symbol""", user_id=session['user_id'], symbol=symbol)

        if ownership[0]['shares'] < shares:
            return apology("insufficient shares owned", 400)

        # Record transaction
        amount = shares * ticker['price']
        db.execute("""INSERT INTO transactions (user_id, symbol, name, shares, price, amount)
                    VALUES (:user_id, :symbol, :name, :shares, :price, :amount)""", user_id=session["user_id"], symbol=ticker['symbol'], name=ticker['name'], shares=-shares, price=ticker['price'], amount=-amount)

        # Access cash data
        rows = db.execute("SELECT cash FROM users WHERE id=:id", id=session["user_id"])
        cash = rows[0]['cash']

        # Update user's cash
        cash = cash + amount
        db.execute("UPDATE users SET cash=:cash WHERE id=:id", cash=cash, id=session["user_id"])

        # Redirect user to home page
        flash("Sale done successfully!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        rows = db.execute("""SELECT symbol FROM transactions WHERE user_id=:id
                          GROUP BY symbol HAVING SUM(shares) > 0""", id=session["user_id"])
        return render_template("sell.html", symbols=[row['symbol'] for row in rows])


def errorhandler(e):
    """Handle error"""

    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
