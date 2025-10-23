from ctypes import cdll, c_char_p
import os
 
dll_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "../build/libhello_c_api.dll"))
# Load the DLL
lib = cdll.LoadLibrary(dll_path)

lib.hello_get_message.restype = c_char_p
msg = lib.hello_get_message()
print(msg.decode())

# Test the say_hello function
lib.hello_say_hello(b"World")

lib.get_info_from_cpp()

