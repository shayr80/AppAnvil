#ifndef TABS_FILECHOOSER_H
#define TABS_FILECHOOSER_H

#include "status.h"

#include <gtkmm/builder.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/enums.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/filechooser.h>
#include <gtkmm/grid.h>
#include <gtkmm/liststore.h>
#include <gtkmm/notebook.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/textview.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treeviewcolumn.h>
#include <memory>
#include <string>
#include <vector>

class FileChooser : public Gtk::ScrolledWindow
{
  public:
    FileChooser();
    void clearLabel();
    
  protected:
    // Signal handler
    void set_l_button_signal_handler(const Glib::SignalProxyProperty::SlotType &func);
    void on_confirm_clicked();

  private:
    // GUI Builder to parse UI from xml file
    Glib::RefPtr<Gtk::Builder> builder;

    // Widgets on the tab  
    std::unique_ptr<Gtk::Box> l_box;
    std::unique_ptr<Gtk::FileChooser> l_filechooser_button;
    std::unique_ptr<Gtk::Label> l_confirm_label;
    std::unique_ptr<Gtk::ComboBoxText> l_cbox;
    std::unique_ptr<Gtk::Button> l_confirm_button;

    //Dialog box
    std::unique_ptr<Gtk::FileChooserDialog> f_diag;

    const std::vector<std::string> col_names{"Profile", "Status"};
    std::shared_ptr<StatusColumnRecord> col_record;

    template <typename T_Widget>
    static std::unique_ptr<T_Widget> get_widget(Glib::ustring name, const Glib::RefPtr<Gtk::Builder>& builder);
};

#endif // TABS_PROFILES_H
