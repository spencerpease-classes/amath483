//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//
// Author: Andrew Lumsdaine
//

#include <future>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

typedef unsigned long size_t;

size_t bank_balance = 300;

static std::mutex atm_mutex;
static std::mutex msg_mutex;

size_t withdraw(const std::string& msg, size_t amount) {
  std::lock(atm_mutex, msg_mutex);
  std::lock_guard<std::mutex> message_lock(msg_mutex, std::adopt_lock);

  std::cout << msg << " withdraws " << std::to_string(amount) << std::endl;

  std::lock_guard<std::mutex> account_lock(atm_mutex, std::adopt_lock);

  bank_balance -= amount;
  return bank_balance;
}

size_t deposit(const std::string& msg, size_t amount) {
  std::lock(atm_mutex, msg_mutex);
  std::lock_guard<std::mutex> account_lock(atm_mutex, std::adopt_lock);

  bank_balance += amount;

  std::lock_guard<std::mutex> message_lock(msg_mutex, std::adopt_lock);

  std::cout << msg << " deposits " << std::to_string(amount) << std::endl;
  return bank_balance;
}

int main() {
  std::cout << "Starting balance is " << bank_balance << std::endl;

  std::future<size_t> bonnie = std::async(std::launch::async, withdraw, "Bonnie", 100);
  std::future<size_t> clyde  = std::async(std::launch::async, deposit, "Clyde", 100);

  std::cout << "Balance after Clyde's deposit is " << clyde.get() << std::endl;
  std::cout << "Balance after Bonnie's withdrawal is " << bonnie.get() << std::endl;
  // std::cout << "Double checking Clyde's deposit " << clyde.get() << std::endl;

  std::cout << "Final bank balance is " << bank_balance << std::endl;

  return 0;
}
