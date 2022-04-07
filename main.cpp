#include <CtrlLib/CtrlLib.h>

#define LAYOUTFILE <ld_decode_gui/decode_gui.lay>
#include <CtrlCore/lay.h>

using namespace Upp;

struct MyAppWindow : public Withdecode<TopWindow> {
	
    MyAppWindow() {
        CtrlLayout(*this, "LD DECODE");
        
        standard_list.Add("NTSC");
        standard_list.Add("NTSC-J");
        standard_list.Add("PAL");
        standard_list.SetIndex(0);
        
        
        
        btn_input_file << [=] {
        	FileSel file;
       		file.ExecuteOpen();
       		if(file.GetFile(0) != "")
       		{
       			//PathFile = file.GetFile(0);
       			text_input_file.SetText(file.GetFile(0));
       			//PromptOK("you have selected (" + PathFile + ")");
       		}
        };
        
        btn_output_folder << [=] {
       		FileSel folder;
       		folder.ExecuteSelectDir();
       		if(folder.GetFile(0) != "")
       		{
       			//PathFolder = folder.GetFile(0);
       			text_output_folder.SetText(folder.GetFile(0));
       			//PromptOK("you have selected (" + PathFolder + ")");
       		}
       };
       
       btn_decode << [=] {
           String command;
           option_dotPy.Get() ? (command = "ld-decode.py --") : (command = "ld-decode.py --");
           command += standard_list.GetValue().ToString() + " --start " + numeric_start.GetText().ToString() + " --length " + numeric_length.GetText().ToString() + " " + text_input_file.GetText().ToString() + " " + text_output_folder.GetText().ToString();
           
        	system("msg %username% " + command);
        	system(command);
       };
    }
};

GUI_APP_MAIN
{
    MyAppWindow().Run();
}

 