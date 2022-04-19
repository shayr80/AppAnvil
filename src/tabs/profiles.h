#ifndef TABS_PROFILES_H
#define TABS_PROFILES_H

#include "status.h"
#include "TrieNode.h"

#include <memory>
#include <string>
#include <vector>

class Profiles : public Status
{
public:
  Profiles();
  virtual void add_data_to_record(const std::string &data);
  void refresh();
  void set_status_change_signal_handler(sigc::slot<void(std::string, std::string, std::string)> change_fun);

  void addWord(TrieNode*&, std::string, Json::Value);
  std::string getCurrPath(std::string);
  std::string setCurrPath(std::string);

protected:
  // Signal handlers
  void on_search_changed();
  void change_status();

private:
  void default_change_fun(const std::string &a, const std::string &b, const std::string &c);

  sigc::slot<void(std::string, std::string, std::string)> profile_status_change_fun;

  const std::vector<ColumnHeader> col_names{ColumnHeader("Profile"), ColumnHeader("Status")};
  std::shared_ptr<StatusColumnRecord> col_record;
};

#endif // TABS_PROFILES_H
