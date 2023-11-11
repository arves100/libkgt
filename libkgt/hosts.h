/**
\file hosts.h
\brief Access to host files
\author Arves100
\date 11/08/2022
\project libkgt
*/
#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Maximum buffer for hosts data
#define HOST_MAX_BUFFER 255 // buffer+1 % 4 -> 0

/**
A single entry of the hosts file
*/
struct KHostsEntry
{
	/// Domain
	char domain[HOST_MAX_BUFFER + 1];

	/// Redirected IP
	char ip[HOST_MAX_BUFFER + 1];

	/// True if the program added this entry
	bool manually_added;

	/// True if the IP is an IPv6
	bool ipv6;
};

/**
Main structure of the hosts file
*/
typedef struct KHOSTS
{
	/// Total length of entries in the HOSTS file
	uint64_t len;

	/// Entries of the hosts file
	struct KHostsEntry* entries;
} KHOSTS;

/**
\param h Host structure
\return true in case of load success, otherwise false

Loads the system host file and stores it inside a host structure
*/
extern bool hosts_load(KHOSTS* h);

/**
\param h Host structure

Frees the host structure
*/
extern void hosts_free(KHOSTS* h);

/**
\param h Host structure
\param domain Domain to search
\return true if it exists, otherwise false

Checks if a domain redirect rule exists or not
*/
extern bool hosts_contains(KHOSTS* h, const char* domain);

/**
\param h Host structure
\param domain Domain to redirect
\param ip New IP
\return true if the entry was successfully added, otherwise false

Adds an entry into the hosts file
*/
extern bool hosts_add(KHOSTS* h, const char* domain, const char* ip);

/**
\param h Host structure
\param domain Domain to get
\return IP of the redirected domain or NULL in case it does not exist

Gets the redirected IP of a domain
*/
extern const char* hosts_get(KHOSTS* h, const char* domain);

/**
\param h Host structure
\return true in case of success, otherwise false

Saves the specified host data into the system host file
*/
extern bool hosts_save(KHOSTS* h);

#ifdef __cplusplus
}
#endif
