/***************************************************************************
 *            berkeley_db_benchmark.cpp
 *
 *  Fri Mar  3 16:33:04 2006
 *  Copyright  2006  Roman Dementiev
 *  Email
 ****************************************************************************/

#include <stxxl>

#include <db_cxx.h>

#define KEY_SIZE 		8
#define DATA_SIZE 	32

#define NODE_BLOCK_SIZE 	(16*1024)
#define LEAF_BLOCK_SIZE 	(32*1024)

//#define TOTAL_CACHE_SIZE 	(5*128*1024*1024/4)
#define TOTAL_CACHE_SIZE    (96*1024*1024)
#define NODE_CACHE_SIZE 	(1*(TOTAL_CACHE_SIZE/5))
#define LEAF_CACHE_SIZE 	(4*(TOTAL_CACHE_SIZE/5))

// BDB settings
u_int32_t    pagesize = 32 * 1024;
u_int        bulkbufsize = 4 * 1024 * 1024;
u_int        logbufsize = 8 * 1024 * 1024;
u_int        cachesize = TOTAL_CACHE_SIZE;
u_int        datasize = DATA_SIZE;
u_int        keysize = KEY_SIZE;
u_int        numitems = 0;


struct my_key
{
	char keybuf[KEY_SIZE];
};

std::ostream & operator << (std::ostream & o, my_key & obj)
{
	for(int i = 0;i<KEY_SIZE;++i)
		o << obj.keybuf[i];
	return o;
}

bool operator == (const my_key & a, const my_key & b)
{
	return strncmp(a.keybuf,b.keybuf,KEY_SIZE) == 0;
}

bool operator != (const my_key & a, const my_key & b)
{
	return strncmp(a.keybuf,b.keybuf,KEY_SIZE) != 0;
}

bool operator < (const my_key & a, const my_key & b)
{
	return strncmp(a.keybuf,b.keybuf,KEY_SIZE) < 0;
}
/*
bool operator > (const my_key & a, const my_key & b)
{
	return strncmp(a.keybuf,b.keybuf,KEY_SIZE) > 0;
}

bool operator <= (const my_key & a, const my_key & b)
{
	return strncmp(a.keybuf,b.keybuf,KEY_SIZE) <= 0;
}

bool operator >= (const my_key & a, const my_key & b)
{
	return strncmp(a.keybuf,b.keybuf,KEY_SIZE) >= 0;
}
*/

struct my_data
{
	char databuf[DATA_SIZE];
};

struct comp_type
{
	bool operator () (const my_key & a, const my_key & b) const
	{
		return strncmp(a.keybuf,b.keybuf,KEY_SIZE) < 0;
	}
	static my_key max_value()
	{ 
		my_key obj;
		memset(obj.keybuf, (std::numeric_limits<char>::max)() , KEY_SIZE);
		return obj;
	}
};
typedef stxxl::map<my_key,my_data,comp_type,NODE_BLOCK_SIZE,LEAF_BLOCK_SIZE> map_type;

void run_bdb_btree(stxxl::int64 ops)
{
	char *filename = "/var/tmp/bdb_file";
	char *letters = "abcdefghijklmnopqrstuvwxuz";
	std::pair<my_key,my_data> element;
    
	unlink(filename);
	
	memset(element.first.keybuf, 'a', KEY_SIZE);
	memset(element.second.databuf, 'b', DATA_SIZE);
	
	Db db(NULL, 0);               // Instantiate the Db object
	
	u_int32_t oFlags = DB_CREATE; // Open flags;
	
	try {
		
		db.set_errfile(stderr);
		db.set_pagesize(pagesize);
    	// Open the database
    	db.open(NULL,                // Transaction pointer 
            filename,          // Database file name 
            NULL,                // Optional logical database name
            DB_BTREE,            // Database access method
            DB_CREATE,              // Open flags
            0);                  // File mode (using defaults)

		
		// here we start with the tests
		
		db.close(0);
	}
	catch(DbException &e)
	{
    	STXXL_ERRMSG("DbException happened")
	} 
	catch(std::exception &e)
	{
    	STXXL_ERRMSG("std::exception happened")
	} 
	
}

void run_stxxl_map(stxxl::int64 ops)
{
	map_type Map(NODE_CACHE_SIZE,LEAF_CACHE_SIZE);
	Map.enable_prefetching();
	
	char *letters = "abcdefghijklmnopqrstuvwxuz";
	std::pair<my_key,my_data> element;
    
	memset(element.first.keybuf, 'a', KEY_SIZE);
	memset(element.second.databuf, 'b', DATA_SIZE);

	
	stxxl::timer Timer;
	stxxl::int64 n_inserts = ops, n_locates=ops, n_range_queries=ops, n_deletes = ops;
	stxxl::int64 i;
	comp_type cmp_;
	
	stxxl::ran32State = 0xdeadbeef;
	
	stxxl::random_number32 myrand;
	
	STXXL_MSG("Records in map: "<<Map.size())
	
	Timer.start();
	            
	for (i = 0; i < n_inserts; ++i)
	{
		element.first.keybuf[(i % KEY_SIZE)] = letters[(myrand() % 26)];
		Map.insert(element);
	}

	Timer.stop();
	STXXL_MSG("Records in map: "<<Map.size())
	STXXL_MSG("Insertions elapsed time: "<<(Timer.mseconds()/1000.)<<
				" seconds : "<< (double(n_inserts)/(Timer.mseconds()/1000.))<<" key/data pairs per sec")

	Timer.reset();

	map_type & CMap(Map); // const map reference
	
	Timer.start();

	for (i = 0; i < n_locates; ++i)
	{
		element.first.keybuf[(i % KEY_SIZE)] = letters[(myrand() % 26)];
		CMap.lower_bound(element.first);
	}

	Timer.stop();
	STXXL_MSG("Locates elapsed time: "<<(Timer.mseconds()/1000.)<<
		" seconds : "<< (double(ops)/(Timer.mseconds()/1000.))<<" key/data pairs per sec")
	
	Timer.reset();
	
	Timer.start();
	
	stxxl::int64 n_scanned = 0, skipped_qieries=0;

	for (i = 0; i < n_range_queries; ++i)
	{
		element.first.keybuf[(i % KEY_SIZE)] = letters[(myrand() % 26)];
		my_key begin_key = element.first;
		map_type::const_iterator begin=CMap.lower_bound(element.first);
		element.first.keybuf[(i % KEY_SIZE)] = letters[(myrand() % 26)];
		map_type::const_iterator beyond=CMap.lower_bound(element.first);
		if(element.first<begin_key)
			std::swap(begin,beyond);
		while(begin!=beyond)
		{
			my_data tmp =  begin->second;
			++n_scanned;
			++begin;
		}
		if(n_scanned >= 10*n_range_queries)	break;
	}
	
	n_range_queries = i;

	Timer.stop();
	STXXL_MSG("Range query elapsed time: "<<(Timer.mseconds()/1000.)<<
		" seconds : "<< (double(n_scanned)/(Timer.mseconds()/1000.))<<
		" key/data pairs per sec, #queries "<< n_range_queries<<" #scanned elements: "<<n_scanned)
	
	
	stxxl::ran32State = 0xdeadbeef;
	memset(element.first.keybuf, 'a', KEY_SIZE);
	memset(element.second.databuf, 'b', DATA_SIZE);
	
	Timer.reset();
	Timer.start();

	for (i = 0; i < n_deletes; ++i)
	{
		element.first.keybuf[(i % KEY_SIZE)] = letters[(myrand() % 26)];
		Map.erase(element.first);
	}

	Timer.stop();
	STXXL_MSG("Records in map: "<<Map.size())
	STXXL_MSG("Erase elapsed time: "<<(Timer.mseconds()/1000.)<<
		" seconds : "<< (double(ops)/(Timer.mseconds()/1000.))<<" key/data pairs per sec")
}


int main(int argc, char * argv[])
{
	if(argc < 3)
	{
		STXXL_MSG("Usage: "<<argv[0]<<" version #ops")
		STXXL_MSG("\t version = 1: test stxxl map")
		STXXL_MSG("\t version = 2: test Berkeley DB btree")
		return 0;
	}

	int version = atoi(argv[1]);
	stxxl::int64 ops = atoll(argv[2]);
	
	switch(version)
	{
		case 1:
			run_stxxl_map(ops);
			break;
		case 2:
			run_bdb_btree(ops);
			break;
		default:
			STXXL_MSG("Unsupported version "<<version)
	}
	
}
