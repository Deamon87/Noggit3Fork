#ifndef MANAGER_H
#define MANAGER_H

#include <string> // std::string
#include <map> // std::map
#include <algorithm> // std::transform()

// base class for manager objects

class ManagedItem 
{
	int refcount;
public:
  //! \todo make this private, create getName().
	std::string name;

  ManagedItem( const std::string& n ) : refcount( 0 )
  {
    name = n;
    std::transform( name.begin(), name.end(), name.begin(), ::tolower );
  }
	virtual ~ManagedItem() { }

  void addref()
  {
    ++refcount;
  }

  bool delref()
  {
    return --refcount == 0;
  }
};

template <class IDTYPE,class MANAGEDITEM>
class Manager
{
public:
	static std::map<std::string, IDTYPE> names;
	static std::map<IDTYPE, MANAGEDITEM*> items;

	static IDTYPE add( const std::string& name );
  
  static void doDelete( IDTYPE id )
  {
    delete items[id];
  }
  
  static void del( IDTYPE id )
  {
    if( items[id]->delref() )
    {
      names.erase( names.find( items[id]->name ) );
      
      doDelete( id );
      
      items.erase( items.find( id ) );
    }
  }
  static void delbyname( const std::string& name )
  {
    std::string name_ = name;
    std::transform( name_.begin(), name_.end(), name_.begin(), ::tolower );
    
    if( has( name_ ) )
    {
      del( get( name_ ) );
    }
  }
  static bool has( const std::string& name )
  {
    std::string name_ = name;
    std::transform( name_.begin(), name_.end(), name_.begin(), ::tolower );
    
    return( names.find( name_ ) != names.end() );
  }
  static IDTYPE get( const std::string& name )
  {
    std::string name_ = name;
    std::transform( name_.begin(), name_.end(), name_.begin(), ::tolower );
    
    return names[name_];
  }

protected:
  static void do_add( const std::string& name, IDTYPE id, MANAGEDITEM* item )
  {
    std::string name_ = name;
    std::transform( name_.begin(), name_.end(), name_.begin(), ::tolower );
    
    names[name_] = id;
    item->addref();
    item->name = name_;
    items[id] = item;
  }
};

#endif
