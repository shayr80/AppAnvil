#include "profiles.h"

#include "jsoncpp/json/json.h"

#include <giomm.h>
#include <glibmm.h>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

// add_data_to_record() is based on assumptions about the output of aa-status.
// If those assumptions are incorrect, or aa-status changes, this could crash.
void Profiles::add_data_to_record(const std::string &data)
{ 
  Json::Value root = Status::parse_JSON(data);
  Json::Value profiles = root["profiles"];

  //set up root for DLB
  TrieNode* dlbRoot = new TrieNode();
  dlbRoot->data = ".";
  dlbRoot->deeper = new std::list<TrieNode>();
  
  col_record->clear();
  for(auto prof = profiles.begin(); prof != profiles.end(); prof++)
  {
    Profiles::addWord(dlbRoot, prof.key().asString(), profiles);
  }
  col_record->reselect_rows();
  refresh();
} 

void Profiles::addWord(TrieNode*& root, std::string key, Json::Value pfs)
{
	std::string path; 
	if (key.at(0) == '/')  path = key.substr(1); //get rid of first '/'
	else 			path = key;
	int N = std::count(path.begin(), path.end(), '/');
	
	TrieNode* currNode = root;
	std::list<TrieNode>* currLayer = currNode->deeper;
	Gtk::TreeRow curr_row = currNode->row;
	for (int i=0; i < N + 1; i++) //for each directory in path
	{
		std::string currPath = getCurrPath(path); //get current directory
		path = setCurrPath(path); //update path
		bool foundDir = false;
		if (currLayer->size() != 0)
		{
			std::list<TrieNode> g = *currLayer;
			std::list<TrieNode>::iterator it;
			//look at each node in the current layer
			for (it = g.begin(); it != g.end(); ++it)
			{
				if ((*it).data.compare(currPath) == 0) //if the nodes match
				{
					foundDir = true;
					currNode = &(*it);
					break;
				}
			}
		}
		
		//if node wasn't found in layer
		if (!foundDir)	
		{
			//make a new node
			TrieNode* newNode = new TrieNode();
			newNode->data = currPath;
			newNode->deeper = new std::list<TrieNode>();
			
			//add child row to root or branch
			Gtk::TreeRow newRow;
			if (i==0) 	newRow = col_record->new_row(); //root
			else		newRow = col_record->new_child_row(curr_row); //branch
			newNode->row = newRow;	
			
			//update visible data
			newRow-> set_value(0, currPath);
			if (i==N)	newRow-> set_value(1, pfs.get(key, UNKNOWN_STATUS).asString());
			
			//push to current list
			currLayer->push_back(*newNode);

			//update current node
			currNode = newNode;
		}	
		//update current layer
		currLayer = currNode->deeper;
		//update current row
		curr_row = currNode->row;
	}
	
}

std::string Profiles::getCurrPath(std::string path)
{
	int ind = path.find("/");
	if (ind == -1) return path;
	return path.substr(0, ind);	// "/" is delimiter
}

std::string Profiles::setCurrPath(std::string path)
{
	int ind = path.find("/");
	if (ind == -1) return path;
	return path.substr(ind+1);	// return path until end
}

void Profiles::change_status()
{
  auto selection = Status::get_view()->get_selection();

  if(selection->count_selected_rows() == 1) {
    auto row = *selection->get_selected();

    std::string profile_path;
    std::string old_status;
    std::string new_status = Status::get_selection_text();

    row->get_value(0, profile_path);
    row->get_value(1, old_status);

    // Convert the status strings to lower case.
    transform(old_status.begin(), old_status.end(), old_status.begin(), ::tolower);
    transform(new_status.begin(), new_status.end(), new_status.begin(), ::tolower);

    this->profile_status_change_fun(profile_path, old_status, new_status);
  } else {
    Status::set_apply_label_text("Please select a row.");
  }
}

void Profiles::refresh()
{
  uint num_visible = col_record->filter_rows();
  Status::set_status_label_text(" " + std::to_string(num_visible) + " matching profiles");
}

void Profiles::default_change_fun(const std::string &a, const std::string &b, const std::string &c)
{
  std::ignore = this;
  std::ignore = a;
  std::ignore = b;
  std::ignore = c;
  std::cerr << "Warning: No signal handler is defined for changing a profile's status." << std::endl;
}

void Profiles::set_status_change_signal_handler(sigc::slot<void(std::string, std::string, std::string)> change_fun)
{
  profile_status_change_fun = std::move(change_fun);
}

Profiles::Profiles() : col_record{StatusColumnRecord::create(Status::get_view(), Status::get_window(), col_names)}
{
  auto refresh_func = sigc::mem_fun(*this, &Profiles::refresh);
  auto apply_func   = sigc::mem_fun(*this, &Profiles::change_status);
  Status::set_refresh_signal_handler(refresh_func);
  Status::set_apply_signal_handler(apply_func);

  auto filter_fun = sigc::mem_fun(*this, &Profiles::filter);
  col_record->set_visible_func(filter_fun);

  sigc::slot<void(std::string, std::string, std::string)> change_fun = sigc::mem_fun(*this, &Profiles::default_change_fun);
  this->set_status_change_signal_handler(change_fun);

  this->show_all();
}
