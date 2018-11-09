/*
 * descriptors.h
 *
 * WinSecCheck.
 *
 * - thesnoom 2018
 */


#ifndef __WSC_DACL_H
#define __WSC_DACL_H

#include <AclAPI.h>

ACL *GetObjectDacl( HANDLE hObject, SE_OBJECT_TYPE ObjectType, SECURITY_DESCRIPTOR **pDescOut );
void PrintPathDacl( ACL *pDacl );

#endif