package main

import (
	"database/sql"
	. "fmt"
	_ "github.com/go-sql-driver/mysql"
)

func main() {
	db, err := sql.Open("mysql", "root:rootpwd@/test?charset=utf8")
	checkErr(err)

	stmt, err := db.Prepare("insert userinfo set username=?, departname=?, created=?")
	checkErr(err)

	res, err := stmt.Exec("kingfree", "gakusei", "2014-1-17")
	checkErr(err)

	id, err := res.LastInsertId()
	checkErr(err)
	Println(id)

	stmt, err = db.Prepare("update userinfo set username=? where uid=?")
	checkErr(err)

	res, err = stmt.Exec("keihou", id)
	checkErr(err)

	affect, err := res.RowsAffected()
	checkErr(err)
	Println(affect)

	rows, err := db.Query("select * from userinfo")
	checkErr(err)

	for rows.Next() {
		var uid int
		var username string
		var departname string
		var created string
		err = rows.Scan(&uid, &username, &departname, &created)
		checkErr(err)
		Println(uid, username, departname, created)
	}

	stmt, err = db.Prepare("delete from userinfo where uid=?")
	checkErr(err)

	res, err = stmt.Exec(id)
	checkErr(err)

	affect, err = res.RowsAffected()
	checkErr(err)
	Println(affect)

	db.Close()
}

func checkErr(err error) {
	if err != nil {
		panic(err)
	}
}
