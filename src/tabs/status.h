#ifndef TABS_STATUS_H
#define TABS_STATUS_H

#include "jsoncpp/json/json.h"
#include "status_column_record.h"

#include <gtkmm/box.h>
#include <gtkmm/builder.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/treeselection.h>
#include <gtkmm/treeview.h>
#include <memory>

constexpr auto UNKNOWN_STATUS = "unknown";

class Status : public Gtk::ScrolledWindow
{
public:
  Status();

  /**
   * @brief Change the text in the label next to the Apply button/spinner.
   */
  void set_apply_label_text(const std::string &str);

protected:
  bool filter(const Gtk::TreeModel::iterator &node);
  virtual bool filter_children(const Gtk::TreeModel::iterator &node);

  /**
   * @brief Decide whether a string should be added to the table
   *
   * @details
   * Takes a string as input and decides whether it should be added the table.
   * Uses the text in the searchbar and the state of the checkboxes to determine
   * whether the string can be added.
   *
   * If `use_regex` is true, then the `match_case` will be ignored.
   *
   * @returns `true` if the string should be added, `false` if it should not
   */
  static bool filter(const std::string &str, const std::string &rule, const bool &use_regex, const bool &match_case,
                     const bool &whole_word);

  /**
   * @brief Change the text in the label directly above the searchbar.
   */
  void set_status_label_text(const std::string &str);

  /**
   * @brief Set the method to be called every time the search area updated.
   *
   * @details
   * Sets the signal handler method to be called every time the search area is updated. This
   * happens when the user clicks a checkbutton, or types into the searchbar.
   *
   * The signal handler should have the following protoype: `void method_name()`
   */
  void set_refresh_signal_handler(const Glib::SignalProxyProperty::SlotType &func);

  /**
   * @brief Set the method to be called every time the apply button is clicked.
   *
   * @details
   * Sets the signal handler method to be called every time the apply button is clicked. This
   * happens when the user clicks the 'Apply' button above the search bar.
   *
   * The signal handler should have the following protoype: `void method_name()`
   */
  void set_apply_signal_handler(const Glib::SignalProxyProperty::SlotType &func);

  /**
   * @brief Return the TreeView associated with this class.
   *
   * @returns The TreeView data member used by this class.
   */
  std::shared_ptr<Gtk::TreeView> get_view();

  /**
   * @brief Return the ScrolledWindow associated with this class.
   *
   * @returns The ScrolledWindow data member used by this class.
   */
  std::shared_ptr<Gtk::ScrolledWindow> get_window();

  /**
   * @brief Return the active selection/choice in the dropdown ComboBoxText
   *
   * @returns The string of the dropdown item which is selected.
   */
  Glib::ustring get_selection_text() const;

  /**
   * @brief Make widgets related to changing profile status invisible to the user.
   *
   * @details
   * Removes the widgets associated with changing a profiles status. This cannot be undone.
   * Widgets that are children of `s_selection_box` will be permanently invisible to the user
   * for this instance of Status.
   */
  void remove_status_selection();

  /**
   * @brief Attempts to parse the passed string to a Json::Value. 
   * Throws an exception if the string cannot be parsed (invalid Json format)
   *
   * @param raw_json String containing Json data to be parsed 
   * @returns A Json::Value parsed from the passed string
   */
  static Json::Value parse_JSON(const std::string &raw_json);

  /**
   * @brief Return the Gtk::SearchEntry associated with this class
   *
   * @returns The Gtk::SearchEntry data member used by this class
   */
  std::shared_ptr<Gtk::SearchEntry> get_search();

  /**
   * @brief Return the Gtk::CheckButton labeled 'Use Regex' associated with this class
   *
   * @returns The Gtk::CheckButton data member named s_use_regex used by this class
   */
  std::shared_ptr<Gtk::CheckButton> get_use_regex();

  /**
   * @brief Return the Gtk::CheckButton labeled 'Match Case' associated with this class
   *
   * @returns The Gtk::CheckButton data member named s_match_case used by this class
   */
  std::shared_ptr<Gtk::CheckButton> get_match_case();

  /**
   * @brief Return the Gtk::CheckButton labeled 'Whole Word' associated with this class
   *
   * @returns The Gtk::CheckButton data member named s_whole_word used by this class
   */
  std::shared_ptr<Gtk::CheckButton> get_whole_word();

  // clang-tidy complains about the `COLUMN_TYPE_STRING` macro, so we assign it here and tell clang-tidy not to look at it
  static constexpr unsigned int COLUMN_TYPE_STRING = G_TYPE_STRING; // NOLINT

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // Container Widgets
  std::shared_ptr<Gtk::TreeView> s_view;
  std::shared_ptr<Gtk::ScrolledWindow> s_win;
  std::unique_ptr<Gtk::Box> s_box;

  // Widgets related to searchbar
  std::shared_ptr<Gtk::SearchEntry> s_search;
  std::shared_ptr<Gtk::CheckButton> s_use_regex;
  std::shared_ptr<Gtk::CheckButton> s_match_case;
  std::shared_ptr<Gtk::CheckButton> s_whole_word;
  std::unique_ptr<Gtk::Label> s_found_label;

  // Widgets related to changing profile status (above search)
  std::unique_ptr<Gtk::Box> s_selection_box;
  std::unique_ptr<Gtk::Button> s_apply_button;
  std::unique_ptr<Gtk::Label> s_apply_info_text;
  std::unique_ptr<Gtk::ComboBoxText> s_status_selection;

  // Misc
  template<typename T_Widget> static std::unique_ptr<T_Widget> get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder);

};

#endif // TABS_STATUS_H
