#ifndef _ld_decode_gui_ld_decode_utils_h_
#define _ld_decode_gui_ld_decode_utils_h_

using namespace Upp;

bool check_file_extention(String ext_list,String ext)
{
	Vector<String> list = Split(ext_list,',');
	
	for(int i=0; i < list.size();i++)
	{	
		if(ext == "." + list[i])
		{
			return true;
		}
	}
	return false;
}

#endif
