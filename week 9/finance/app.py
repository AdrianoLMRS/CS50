import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

import datetime

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


def getUserCash(userId: int) -> float:
    userCashDb = db.execute("SELECT cash FROM users WHERE id = ?", userId)
    return float(userCashDb[0]["cash"])


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # select user's stock portfolio and cash total
    userId = session["user_id"]
    purchases = db.execute(
        "SELECT stockSymbol, SUM(numShares) AS shares, price, date FROM purchases WHERE userId = ? GROUP BY stockSymbol",
        userId,
    )
    cash = getUserCash(userId)

    return render_template("index.html", database=purchases, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        shares_str = request.form.get("shares")
        if not shares_str or not shares_str.isdigit():
            return apology("Please provide a positive integer for shares.")

        shares = int(shares_str)
        if shares <= 0:
            return apology("Please provide a positive number of shares.")

        # See if symbol exists
        stock = lookup(symbol)
        if stock == None:
            return apology("Stock symbol does not exist...")

        # See user cash in users table
        userId = session["user_id"]  # get user id
        purchaseValue = int(shares) * float(stock["price"])

        userCash = getUserCash(userId)

        if userCash < purchaseValue:
            return apology("User does not have enough money...")
        updatedCash = userCash - purchaseValue

        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   updatedCash, userId)  # Update user cash

        date = datetime.datetime.now()
        db.execute(
            "INSERT INTO purchases (userId, stockSymbol, numShares, price, date) VALUES (?, ?, ?, ?, ?)",
            userId, stock["symbol"], shares, stock["price"], date,
        )

        flash("Congrats!  Purchase successful!")
        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    userId = session["user_id"]
    purchasesDb = db.execute("SELECT * FROM purchases WHERE userId = ? ORDER BY date DESC", userId)

    return render_template("history.html", purchases=purchasesDb)


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
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

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
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Must give a stock symbol")

        stock = lookup(symbol)

        if stock == None or not stock:
            return apology("Stock symbol non-existent")

        return render_template("stockInfo.html",
                               name=stock["name"],
                               price=stock["price"],
                               symbol=stock["symbol"],
                               )
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirm = request.form.get("confirmation")

        if not username:
            return apology("Must Give Username")
        if not password:
            return apology("Must Give Password")
        if not confirm:
            return apology("Must Give confirmation password")
        if password != confirm:
            return apology("Passwords Do Not Match")

        hash = generate_password_hash(password)

        try:
            new_user = db.execute(
                "INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)
        except:
            return apology("Username already exists")

        session["user_id"] = new_user

        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        shares_str = request.form.get("shares")
        if not shares_str or not shares_str.isdigit():
            return apology("Please provide a positive integer for shares.")

        shares = int(shares_str)
        if shares <= 0:
            return apology("Please provide a positive number of shares.")

        stock = lookup(symbol)
        if stock == None:
            return apology("Symbol Does Not Exist")

        sellValue = shares * stock["price"]

        userId = session["user_id"]

        userCash = getUserCash(userId)

        userSharesDb = db.execute(
            """
            SELECT SUM(numShares) AS totalShares FROM purchases
            WHERE userId = ? AND stockSymbol = ?
            """,
            userId, symbol,
        )
        userShares = userSharesDb[0]["totalShares"]

        if shares > userShares:
            return apology("You dont have this amount of shares...")

        updatedCash = userCash + sellValue

        db.execute("UPDATE users SET cash = ? WHERE id = ?", updatedCash, userId)

        date = datetime.datetime.now()
        db.execute(
            "INSERT INTO purchases (userId, stockSymbol, numShares, price, date) VALUES (?, ?, ?, ?, ?)",
            userId, stock["symbol"], (-1) * shares, stock["price"], date,
        )

        flash("Shares sold.")
        return redirect("/")
    else:
        userId = session["user_id"]
        symbols = db.execute(
            "SELECT stockSymbol, price FROM purchases WHERE userId = ? GROUP BY stockSymbol HAVING SUM(numShares) > 0",
            userId,
        )
        print(symbols)
        return render_template("sell.html", symbols=symbols)


@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash():
    """Add user cash"""
    if request.method == "POST":
        formCash = float(request.form.get("cash"))

        if not formCash or formCash <= 0:
            return apology("Please provide a valid amount of cash...")

        userId = session["user_id"]
        userCash = getUserCash(userId)

        updatedCash = userCash + formCash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", updatedCash, userId)

        flash(f"Cash added!  Total - ${updatedCash:,.2f}")
        return redirect("/")

    else:
        userId = session["user_id"]
        userCash = getUserCash(userId)

        return render_template("cash.html", cash=userCash)
