//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <iostream>
#include <mutex>
#include <string>
#include <thread>

int bank_balance = 300;

class transaction {
public:
  transaction(int _sign, int& _balance) : sign(_sign), balance(_balance) {}

  void operator()(const std::string& msg, int amount) {
    std::lock_guard<std::mutex> tr_lock(tr_mutex);
    std::cout << msg << ": " << sign * amount;
    std::cout << " -- Balance: " << balance << std::endl;
    balance += sign * amount;
  };

  void operator()(int amount) {
    std::lock_guard<std::mutex> tr_lock(tr_mutex);
    balance += sign * amount;
  };

private:
  int               sign;
  int&              balance;
  static std::mutex tr_mutex;    // Note static -- there can be only one
};

std::mutex transaction::tr_mutex;

int main() {
  transaction deposit(1, bank_balance);      // define an object named deposit
  transaction withdraw(-1, bank_balance);    // define an object named withdraw

  std::cout << "Initial balance is " << bank_balance << std::endl;

  std::thread bonnie(withdraw, "Bonnie", 100);
  std::thread clyde(deposit, "Clyde", 100);

  bonnie.join();
  clyde.join();

  std::cout << "Final bank balance is " << bank_balance << std::endl;

  return 0;
}
