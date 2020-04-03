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
 
#define ClientKey 1234 
 
typedef struct stu_msgbuf_s
{
					long msgtype;
						char msgtext[1024];
}stu_msgbuf_t;
 
int sendmsg(int msgid ,const char * pchBuf, int len,int msgtype)
{
					stu_msgbuf_t stuSendMsg = {0};
						int ret = -1;
						 
							if(NULL == pchBuf ||
																	strlen(pchBuf) == 0 ||
																			len <=0)
												{
																	    printf("\nparam error [sendmsg]\n");
																					return -1;
																						}
							 
							 
								stuSendMsg.msgtype = msgtype;
									memset(stuSendMsg.msgtext,0,sizeof(stuSendMsg.msgtext));
										strncpy(stuSendMsg.msgtext,pchBuf,sizeof(stuSendMsg.msgtext)-1);
										 
											ret = msgsnd(msgid,&stuSendMsg,sizeof(stuSendMsg.msgtext),IPC_NOWAIT);
												if(-1 == ret)
																	{
																							printf("\n msgsnd gs_nClientMsgid [%d] errno=%d  [%s]\n",msgid,errno,strerror(errno));
																									return -1;
																										}
													else
																		{
																								printf("\nmsg snd  [%s]\n",stuSendMsg.msgtext);
																									}
														
														return 0;
}
 
int init(int *pMsgId)
{
					int msgid = -1;
						key_t ktmp = ftok("/etc/passwd",ClientKey);
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
																						int type = 0;
																								static int i = 0;
																										type = i%3+1;
																												
																												i++;
																														printf("\ntype[%d]\n",type);
																																snprintf(chStr,sizeof(chStr),"type[%d]--[%ld]",type,time(NULL));
																																		sendmsg(msgid,chStr, strlen(chStr)+1,type);
																																				sleep(2);
																																					}
										
}
