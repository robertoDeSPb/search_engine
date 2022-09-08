# search_engine

THIS IS MY SEARCH_ENGINE PROJECT!


For be sure that it works, you need pre-installed boost library with MSVC compiller.

Than you should make a couple of edits (if your path to the boost not match mine) in CMakeLists.txt in search_engine folder, like this:

set(Boost_INCLUDE_DIR "C:\\boost_1_79_0")
set(Boost_LIBRARY_DIR "C:\\boost_1_79_0\\stage\\lib")


If you want to add new file for future ranking, add your .txt file to test_files folder. File should have a name like file001.txt. Then add path to this file in config.json in json_files folder, like this:

"files": [
    "../test_files/file001.txt",
    "../test_files/file002.txt",
    "../test_files/file003.txt",
    "../test_files/file004.txt"
  ]
  
  
