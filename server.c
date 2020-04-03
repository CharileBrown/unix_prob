#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
 
extern int errno;
 
#define lientKey 1234
 
static int  gs_nClientMsgid = -1; 
 
typedef struct stu_msgbuf_s
{
					long msgtype;
						char msgtext[1024];
}stu_msgbuf_t;
 
int rcvmsg(int msgid,const char * pchBuf, int len, int nMsgType)
{
					stu_msgbuf_t stuSendMsg = {0};
						int ret = -1;
						 
							stuSendMsg.msgtype = nMsgType;
								ret = msgrcv(msgid, &stuSendMsg, sizeof(stuSendMsg.msgtext), stuSendMsg.msgtype ,IPC_NOWAIT);
									if(-1 == ret)
														{
																				printf("\n msgrcvg s_nClientMsgid [%d] errno=%d  [%s]\n", msgid ,errno,strerror(errno));
																						return -1;
																							}
										else
															{
																					printf("\n msgrcv  [%s]\n",stuSendMsg.msgtext);
																						}
											
											return 0;
}
 
int init(int *pMsgId)
{
					int msgid = -1;
						key_t ktmp = ftok("/etc/passwd",lientKey);
							if(ktmp == -1)
												{
																		printf("\n if(ktmp == -1) errno=%d  [%s]\n",errno,strerror(errno));
																				return -1;
																					}
								else
													{
																			printf("\n ftok ktmp[%d]\n",ktmp);
																				}
									
									msgid = msgget(ktmp, 0666|IPC_CREAT);
									 
										if(msgid < 0)
															{
																					printf("\n if(gs_nClientMsgid == -1) errno=%d  [%s]\n",errno,strerror(errno));
																							return -1;
																								}
										 
											printf("\n msgget sucess %d  ktmp[%d]\n",msgid,ktmp);
											 
												*pMsgId = msgid;
													return 0;
													 
}
 
int main()
{
					int ret =-1;
						int msgid =-1;
							ret = init(&msgid);
								if(ret == -1)
													{
																			return 0;
																				}
									while(1)
														{
																				char chStr[20] = {0};
																						int msgtype = 2;
																								
																								//printf("\ni[%d]\n",i);
																								//snprintf(chStr,sizeof(chStr),"%d --- %ld",i,time(NULL));
																								rcvmsg(msgid,chStr, strlen(chStr)+1,msgtype);
																										sleep(2);
																											}
										
}
 
