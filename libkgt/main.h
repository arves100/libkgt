/**
\file main.h
\brief Main wrapper
\author Arves100
\date 14/06/2023
\project libkgt
*/
#pragma once

#ifndef KGT_NO_MAIN_DEF
#define main kgt_main
#endif

/**
\param argc Number of arguments
\param argv Argument data
\return exit code

Main wrapped application
*/
extern KGT_WEAK int kgt_main(int argc, char** argv);


/**
Delay kgt initialization for modules that does not use bundled main
\return true if the initialization was successfull, otherwise false
*/
extern bool kgt_delay_init(void);

/**
Delay kgt free for modules that does not use bundled main
*/
extern void kgt_delay_free(void);
