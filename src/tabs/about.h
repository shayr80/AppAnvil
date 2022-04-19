#ifndef TABS_ABOUT_H
#define TABS_ABOUT_H

#include "status.h"

#include <gtkmm/builder.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/enums.h>
#include <gtkmm/fixed.h>
#include <gtkmm/grid.h>
#include <gtkmm/liststore.h>
#include <gtkmm/notebook.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/textview.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treeviewcolumn.h>
#include <gtkmm/viewport.h>
#include <memory>
#include <string>
#include <vector>

class About : public Gtk::ScrolledWindow
{
public:
  About();

protected:
  // Signal handler 
  void set_link_btn_signal_handler(const Glib::SignalProxyProperty::SlotType &func);
  void on_confirm_clicked();
  void set_search_signal_handler(const Glib::SignalProxyProperty::SlotType &func);
  void on_search_changed();
  std::string get_marked_up_text();
  std::string get_marked_up_text_lower();

private:
  // GUI Builder to parse UI from xml file
  Glib::RefPtr<Gtk::Builder> builder;

  // Widgets on the tab
  std::unique_ptr<Gtk::Box> a_box;
  std::unique_ptr<Gtk::ScrolledWindow> a_win;
  std::unique_ptr<Gtk::Viewport> a_view;
  std::unique_ptr<Gtk::Label> a_label;
  std::unique_ptr<Gtk::Box> a_box_hz;
  std::unique_ptr<Gtk::Button> link_btn;
  std::unique_ptr<Gtk::SearchEntry> a_search;

  template<typename T_Widget> static std::unique_ptr<T_Widget> get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder> &builder);
};

#endif // TABS_ABOUT_H
