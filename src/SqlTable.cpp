#include "SqlTable.hpp"

void SqlTable::removeItem(const int id) const {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, ("DELETE FROM " + tableName + " WHERE ID = ?;").c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

SqlTable::SqlTable(std::string name, const std::string &createQuery) : tableName(name) {
    sqlite3_open(name.replace(0, 1, 1, static_cast<char>(tolower(name[0]))).append(".db").c_str(), &db);
    query("CREATE TABLE IF NOT EXISTS " + tableName + "(Id INTEGER PRIMARY KEY AUTOINCREMENT, " + createQuery + ")");
}

SqlTable::~SqlTable() {
    sqlite3_close(db);
}

void SqlTable::query(const std::string &query) const {
    sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);
}
