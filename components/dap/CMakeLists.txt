idf_component_register(SRCS 
                            "src/dap_main.c"
                            "src/dap_protocol.c"
                            "src/dap_hw.c"
                        INCLUDE_DIRS 
                            "." "include"
                        REQUIRES 
                            "driver"
                            "tinyusb"
)
