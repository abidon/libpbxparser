# libpbxparser
C++ library to parse old-style plist files (used in Xcode project files)

* How to build: `make`
* How to use: `cp *.{a,hpp} ~/my-super-project`
* Example snippet (prints the root object uid of an xcode project file):

```c++
#include <fstream>
#include <iostream>
#include <string>

#include "plist_decoder.hpp"

int main(int argc, const char * argv[]) {
	std::ifstream file("MySuperProject.xcodeproj/project.pbxproj");
	
	std::string content;
	content.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	
	pbx::Dictionary document = pbx::PlistDecoder::parse(content);
	
	std::cout << "Root Object UID: " << document["rootObject"].string_value() << '\n';
	
    return 0;
}
```