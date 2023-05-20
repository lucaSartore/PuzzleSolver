# communication protocol

this file describe the communication protocol between the c# and the c++ code.

## functionalities

a list of the functionality offered bt the C dll

### I/O functionality

 - `open_folder`: open a folder as a project
 - `create_new`: create a new project in a folder
 - `save_as`: save the current project in a folder
 - `free_memory`: deallocate the entire puzzle solver class

### config functionality
 
 - `set_number_of_cores`: specify the number of cores the program will use
 - `set_split_threshold`: specify the threshold used to split the pieces
 - `enable_preview`: enable preview functionalities
 - `disable_preview`: disable preview functionalities

### preview functionality

 - `next_preview_image`: go to the next image to preview. and return a struct pointing to an image with an uchar array
 - `get_threshodl_preview`: return an image that show how well the applied threshold is working

### control functionality

 - `get_state`: return the state the machine is currently in
 - `go_to_next_stae`: go to the next state in the list of tings to do