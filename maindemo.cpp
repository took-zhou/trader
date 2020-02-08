////#include "stdafx.h"
//#include "main.h"
//#include "traderSpi.h"
//#include "base64.h"
//#include <semaphore.h>
//#include "log.h"
//extern FILE *outfile_demo;
//extern sem_t sem;
//extern sem_t sem2;
//extern sem_t sem_login;
//extern vector<string> vector_OrderSysID;
//int main()
//{
//	//system("COLOR 0A");
//	//logfile = fopen("syslog.txt", "w");
//
//    printf("********NEW TEST*********");
//	/******获取系统信息*****/
//    char systeminfor[344];
//    int n_len;
//    int uu = CTP_GetSystemInfo(systeminfor,n_len);
//    std::cout << uu << std::endl;
//    std::cout << systeminfor << std::endl;
//    char decoded[344];
//    char *aa = decode(systeminfor,344, decoded,344);
//    printf("system information is:%s\n", aa);
//
//	/***下单撤单文件***/
//
//
//	/******信号初始化********/
//    sem_init(&sem, 0, 0);
//    sem_init(&sem2, 0, 0);
//    sem_init(&sem_login, 0, 0);
//
//	string g_chFrontaddr = getConfig("trade", "FrontAddr");
//	cout << "g_chFrontaddr = " << g_chFrontaddr << "\n" << endl;
//	CTraderApi *pUserApi = new CTraderApi;//--------------------------
//	pUserApi->CreateFtdcTraderApi("trade_flow/");
//	LOGDEMO(pUserApi->GetApiVersion());
//	cout << endl;
//	CSimpleHandler sh(pUserApi);
//cir:pUserApi->RegisterSpi(&sh);
//	pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);
//	pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);
//	pUserApi->RegisterFront(const_cast<char *>(g_chFrontaddr.c_str()));
//	pUserApi->Init();
//	//WaitForSingleObject(g_hEvent, INFINITE);
//    sem_wait(&sem);
//	//_getch();
//	while (true)
//	{
//		LOGDEMO("请确定连接模式:\n");
//		LOGDEMO("1.直连模式\n");
//		LOGDEMO("2.中继服务器操作员模式(一对多模式)\n");
//		LOGDEMO("3.中继服务器非操作员模式(多对多模式)\n");
//		int mode_num;
//		cin >> mode_num;
//		switch (mode_num)
//		{
//		case 1://直连模式
//		{
//			sh.ReqAuthenticate();
//			//WaitForSingleObject(g_hEvent, INFINITE);
//            sem_wait(&sem);
//			sh.ReqUserLogin();
//			//WaitForSingleObject(g_hEvent, INFINITE);
//            sem_wait(&sem_login);
//			break;
//		}
//		case 2://操作员模式
//		{
//			sh.ReqAuthenticate();
//			//WaitForSingleObject(g_hEvent, INFINITE);
//            sem_wait(&sem);
//			sh.ReqUserLogin();
//			//WaitForSingleObject(g_hEvent, INFINITE);
//            sem_wait(&sem);
//			sh.SubmitUserSystemInfo();
//			break;
//		}
//		case 3://非操作员模式
//		{
//			sh.ReqAuthenticate();
//			//WaitForSingleObject(g_hEvent, INFINITE);
//            sem_wait(&sem);
//			sh.RegisterUserSystemInfo();
//			sh.ReqUserLogin();
//			//WaitForSingleObject(g_hEvent, INFINITE);
//            sem_wait(&sem);
//			break;
//		}
//		default:
//			LOGDEMO("选择的模式有误，请重新输入！\n");
//            getchar();
//            getchar();
//
//			system("clear");
//		}
//
//		break;
//	}
//
//    /*getchar();
//    getchar();*/
//
//
//	LOGDEMO("日期:");
//	LOGDEMO(pUserApi->GetTradingDay());
//	cout << endl;
//	while (true)
//	{
//	loop:int input1;
//		system("clear");
//		LOGDEMO("201.上报用户终端信息\n");
//		LOGDEMO("110,交易查询合约行情收行情\n");
//		LOGDEMO("101.用户登录请求\n");
//		LOGDEMO("102.客户端认证\n");
//		LOGDEMO("103.请求登出\n");
//		LOGDEMO("1.结算单确认请求\n");
//		LOGDEMO("2.用户口令更新请求\n");
//		LOGDEMO("3.资金账户口令更新请求\n");
//		LOGDEMO("/////////////报单////////////\n");
//		LOGDEMO("4.报单模块\n");
//		LOGDEMO("////////////查询/////////////\n");
//		LOGDEMO("5.查询模块\n");
//		LOGDEMO("/////////////行权&做市商////////////\n");
//		LOGDEMO("6.行权&做市商\n");
//		LOGDEMO("///////////出入金/////////////\n");
//		LOGDEMO("7.转账指令\n");
//		LOGDEMO("///////////铜期权测试////////////\n");
//		LOGDEMO("8.铜期权测试\n");
//		LOGDEMO("9.版本6.3.13新加接口\n");
//		LOGDEMO("0.清空界面\n");
//		LOGDEMO("100.退出程序\n");
//		LOGDEMO("请输入你需要的操作序号：");
//
//		cin >> input1;
//		switch (input1)
//		{
//		case 201:
//		{
//			sh.SubmitUserSystemInfo();
//            getchar();
//            getchar();
//
//			break;
//		}
//		case 110:
//		{
//			string g_chFrontMdaddr = getConfig("trade", "FrontMdAddr");
//			cout << "g_chFrontMdaddr = " << g_chFrontMdaddr << "\n" << endl;
//			CThostFtdcMdApi  *pUserMdApi =
//				CThostFtdcMdApi::CreateFtdcMdApi("market_flow/");
//			CSimpleMdHandler ash(pUserMdApi);
//			pUserMdApi->RegisterSpi(&ash);
//			pUserMdApi->RegisterFront(const_cast<char *>(g_chFrontMdaddr.c_str()));
//			pUserMdApi->Init();
//			//WaitForSingleObject(xinhao, INFINITE);
//            sem_wait(&sem2);
//			sh.ReqQryInstrument();//查询合约
//			//WaitForSingleObject(xinhao, INFINITE);
//            sem_wait(&sem2);
//			ash.SubscribeMarketData();//订阅行情
//            getchar();
//            getchar();
//
//			pUserMdApi->Release();
//			break;
//		}
//		case 101:
//		{
//			sh.ReqUserLogin();
//            getchar();
//            getchar();
//
//			break;
//		}
//		case 102:
//		{
//			sh.ReqAuthenticate();
//            getchar();
//            getchar();
//
//			break;
//		}
//		case 103:
//		{
//			sh.ReqUserLogout();
//            getchar();
//            getchar();
//
//			goto cir;
//		}
//		case 1://结算单确认请求
//		{
//			sh.ReqSettlementInfoConfirm();//结算单确认请求
//			//WaitForSingleObject(g_hEvent, INFINITE);
//            sem_wait(&sem);
//            getchar();
//            getchar();
//
//			system("clear");
//			break;
//		}
//		case 2://用户口令更新请求
//		{
//			sh.ReqUserPasswordUpdate();//用户口令更新请求
//			//WaitForSingleObject(g_hEvent, INFINITE);
//            sem_wait(&sem);
//            getchar();
//            getchar();
//
//			system("clear");
//			break;
//		}
//		case 3://资金账户口令更新请求
//		{
//			sh.ReqTradingAccountPasswordUpdate();//资金账户口令更新请求
//			//WaitForSingleObject(g_hEvent, INFINITE);
//            sem_wait(&sem);
//            getchar();
//            getchar();
//
//			system("clear");
//			break;
//		}
//		case 4://报单录入请求
//		{
//			outfile_demo = fopen("order_record.log", "a");
//		orderinsert:system("clear");
//			int orderinsert_num;
//			LOGDEMO("4.报入一笔立即单\n");
//			LOGDEMO("5.撤销上一笔报单\n");
//			LOGDEMO("6.报入预埋单-限价单立即单\n");
//			LOGDEMO("7.撤销预埋单-(上一个预埋单)\n");
//			LOGDEMO("8.报入预埋撤单\n");
//			LOGDEMO("9.撤销预埋撤单-(上一个预埋撤单)\n");
//			LOGDEMO("10.报入条件单\n");
//			LOGDEMO("11.撤销条件单-(上一个条件单)\n");
//			LOGDEMO("25.大商所止损单\n");
//			LOGDEMO("26.大商所止盈单\n");
//			LOGDEMO("27.FOK全成全撤\n");
//			LOGDEMO("28.FAK部成部撤\n");
//			LOGDEMO("29.市价单\n");
//			LOGDEMO("30.套利指令\n");
//			LOGDEMO("31.互换单\n");
//			LOGDEMO("0.返回上一层\n");
//			cin >> orderinsert_num;
//			switch (orderinsert_num)
//			{
//			case 0:
//				fclose(outfile_demo);
//				goto loop;
//			case 4://报入普通立即单
//			{
//				fprintf(outfile_demo,"开始报单:\n");
//				int frequence = 0;
//				LOGDEMO("请输入报单频率:\n");
//				cin >> frequence;
//				//sh.ReqOrderInsert_Ordinary();//报入普通立即单
//				//_getch();
//				//break;
//				for (int a = 0; a < frequence; a++)
//				{
//					sh.ReqOrderInsert_Ordinary();
//                    printf("报单录入请求已发送，等待响应。。。。。。\n");
//				}
//                getchar();
//                getchar();
//                 ;
//                break;
//
//			}
//			case 5://撤销上一笔报单
//			{
//				sh.ReqOrderAction_Ordinary();//撤销上一笔报单
//                getchar();
//                getchar();
//                 ;
//				break;
//			}
//			case 6://报入预埋单
//			{
//				sh.ReqParkedOrderInsert();//报入预埋单
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 7://删除预埋单
//			{
//				sh.ReqRemoveParkedOrder();//删除预埋单
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 8://报入预埋撤单
//			{
//				sh.ReqParkedOrderAction();//报入预埋撤单
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 9://删除预埋单撤单
//			{
//				sh.ReqRemoveParkedOrderAction();//删除预埋单撤单
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 10://报入条件单
//			{
//			it:LOGDEMO("1.最新价大于条件价\n");
//				LOGDEMO("2.最新价大于等于条件价\n");
//				LOGDEMO("3.最新价小于条件价\n");
//				LOGDEMO("4.最新价小于等于条件价\n");
//				LOGDEMO("5.卖一价大于条件价\n");
//				LOGDEMO("6.卖一价大于等于条件价\n");
//				LOGDEMO("7.卖一价小于条件价\n");
//				LOGDEMO("8.卖一价小于等于条件价\n");
//				LOGDEMO("9.买一价大于条件价\n");
//				LOGDEMO("10.买一价大于等于条件价\n");
//				LOGDEMO("11.买一价小于条件价\n");
//				LOGDEMO("12.买一价小于等于条件价\n");
//				LOGDEMO("13.返回上一层\n");
//				LOGDEMO("请输入你需要报入的条件单类型:\n");
//				int num;
//				cin >> num;
//				if (num < 1 || num>13)
//				{
//					LOGDEMO("输入的序号有误,请重新输入.\n");
//                    getchar();
//                    getchar();
//
//					goto it;
//				}
//				else if (num == 13)
//				{
//					goto orderinsert;
//				}
//				else
//				{
//					sh.ReqOrderInsert_Condition(num);//报入条件单
//                    getchar();
//                    getchar();
//
//					break;
//				}
//			}
//			case 11://报入条件单撤单
//			{
//				sh.ReqOrderAction_Condition();//报入条件单撤单
//                getchar();
//                getchar();
//
//				break;
//			}
//
//			case 25://大商所止损单
//			{
//				sh.ReqOrderInsert_Touch();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 26://大商所止盈单
//			{
//				sh.ReqOrderInsert_TouchProfit();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 27://FOK全成全撤
//			{
//				sh.ReqOrderInsert_VC_CV();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 28://FAK部成部撤
//			{
//				sh.ReqOrderInsert_VC_AV();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 29://市价单
//			{
//				sh.ReqOrderInsert_AnyPrice();
//                getchar();
//                getchar();
//                 ;
//				break;
//			}
//			case 30://套利指令
//			{
//				sh.ReqOrderInsert_Arbitrage();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 31://互换单
//			{
//				sh.ReqOrderInsert_IsSwapOrder();
//                getchar();
//                getchar();
//
//				break;
//			}
//			default:
//				LOGDEMO("请输入正确的序号\n");
//                getchar();
//                getchar();
//
//				goto orderinsert;
//			}
//
//			goto orderinsert;
//		}
//		case 5://查询界面
//		{
//		search:system("clear");
//			int choose_num;
//			LOGDEMO("11.查询成交\n");
//			LOGDEMO("12.查询预埋单\n");
//			LOGDEMO("13.查询预埋撤单\n");
//			LOGDEMO("14.查询报单\n");
//			LOGDEMO("15.撤单对应查询编号\n");
//			LOGDEMO("16.请求查询资金账户\n");//ReqQryTradingAccount
//			LOGDEMO("17.请求查询投资者持仓\n");//ReqQryInvestorPosition
//			LOGDEMO("18.请求查询投资者持仓明细\n");//ReqQryInvestorPositionDetail
//			LOGDEMO("19.请求查询交易所保证金率\n");//ReqQryExchangeMarginRate
//			LOGDEMO("20.请求查询合约保证金率\n");//ReqQryInstrumentMarginRate
//			LOGDEMO("21.请求查询合约手续费率\n");//ReqQryInstrumentCommissionRate
//			LOGDEMO("22.请求查询做市商合约手续费率\n");//ReqQryMMInstrumentCommissionRate
//			LOGDEMO("23.请求查询做市商期权合约手续费\n"); //ReqQryMMOptionInstrCommRate
//			LOGDEMO("24.请求查询报单手续费\n");//ReqQryInstrumentOrderCommRate
//			LOGDEMO("25.请求查询期权合约手续费\n");//ReqQryOptionInstrCommRate
//			LOGDEMO("26.请求查询合约\n");//ReqQryInstrument
//			LOGDEMO("27.请求查询投资者结算结果\n");//ReqQrySettlementInfo
//			LOGDEMO("28.请求查询转帐流水\n");//ReqQryTransferSerial
//			LOGDEMO("29.请求查询询价\n");
//			LOGDEMO("30.请求查询报价\n");
//			LOGDEMO("31.请求查询执行宣告\n");
//			LOGDEMO("32.请求查询转帐银行\n");
//			LOGDEMO("33.请求查询交易通知\n");
//			LOGDEMO("34.请求查询交易编码\n");
//			LOGDEMO("35.请求查询结算信息确认\n");
//			LOGDEMO("36.请求查询产品组\n");
//			LOGDEMO("37.请求查询投资者单元\n");
//			LOGDEMO("38.期货发起查询银行余额请求\n");
//			LOGDEMO("39.请求查询经纪公司交易参数\n");
//			LOGDEMO("0.返回上一层\n");
//			LOGDEMO("请输入选择的序号:\n");
//			cin >> choose_num;
//			switch (choose_num)
//			{
//			case 11://请求查询成交
//			{
//				sh.ReqQryTrade();//请求查询成交
//                printf("查询请求已发出。\n");
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 12://请求查询服务器预埋单
//			{
//				sh.ReqQryParkedOrder();//请求查询服务器预埋单
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 13://请求查询服务器预埋撤单
//			{
//				sh.ReqQryParkedOrderAction();//请求查询服务器预埋撤单
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 14://请求查询报单
//			{
//				sh.ReqQryOrder();//请求查询报单
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 15://撤销查询的报单回报
//			{
//			action:int action_num;
//				LOGDEMO("请输入需要撤单的序号：\n");
//				cin >> action_num;
//				LOGDEMO("%d\n", action_num);
//				if (action_num < 1 || action_num > vector_OrderSysID.size())
//				{
//					LOGDEMO("输入的序号有误,请重新输入.\n");
//                    getchar();
//                    getchar();
//
//					goto action;
//				}
//				sh.ReqOrderAction_forqry(action_num);//撤销查询的报单回报
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 16://请求查询资金账户
//			{
//				sh.ReqQryTradingAccount();//请求查询资金账户
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 17://请求查询投资者持仓
//			{
//				sh.ReqQryInvestorPosition();//请求查询投资者持仓
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 18://请求查询投资者持仓明细
//			{
//				sh.ReqQryInvestorPositionDetail();//请求查询投资者持仓明细
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 19://请求查询交易所保证金率
//			{
//				sh.ReqQryExchangeMarginRate();//请求查询交易所保证金率
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 20://请求查询合约保证金率
//			{
//				sh.ReqQryInstrumentMarginRate();//请求查询合约保证金率
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 21://请求查询合约手续费率
//			{
//				sh.ReqQryInstrumentCommissionRate();//请求查询合约手续费率
//                getchar();
//                getchar();
//                 ;
//				break;
//			}
//			case 22://请求查询做市商合约手续费率
//			{
//				sh.ReqQryMMInstrumentCommissionRate();//请求查询做市商合约手续费率
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 23://请求查询做市商期权合约手续费
//			{
//				sh.ReqQryMMOptionInstrCommRate();//请求查询做市商合约手续费率
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 24://请求查询报单手续费
//			{
//				sh.ReqQryInstrumentOrderCommRate();//请求查询报单手续费
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 25://请求查询期权合约手续费
//			{
//				sh.ReqQryOptionInstrCommRate();//请求查询期权合约手续费
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 26://请求查询合约
//			{
//				sh.ReqQryInstrument();//请求查询合约
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 27://请求查询投资者结算结果
//			{
//				sh.ReqQrySettlementInfo();//请求查询投资者结算结果
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 28://请求查询转帐流水
//			{
//				sh.ReqQryTransferSerial();//请求查询投资者结算结果
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 29://请求查询询价
//			{
//				sh.ReqQryForQuote();//请求查询询价
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 30://请求查询报价
//			{
//				sh.ReqQryQuote();//请求查询报价
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 31://请求查询执行宣告
//			{
//				sh.ReqQryExecOrder();//请求查询执行宣告
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 32://请求查询转帐银行
//			{
//				sh.ReqQryTransferBank();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 33://请求查询交易通知
//			{
//				sh.ReqQryTradingNotice();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 34://请求查询交易编码
//			{
//				sh.ReqQryTradingCode();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 35://请求查询结算信息确认
//			{
//				sh.ReqQrySettlementInfoConfirm();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 36://请求查询产品组
//			{
//				sh.ReqQryProductGroup();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 37://请求查询投资者单元
//			{
//				sh.ReqQryInvestUnit();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 38://期货发起查询银行余额请求
//			{
//				sh.ReqQueryBankAccountMoneyByFuture();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 39://请求查询经纪公司交易参数
//			{
//				sh.ReqQryBrokerTradingParams();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 0:
//			{
//				goto loop;
//			}
//			default: {
//				LOGDEMO("请输入正确的序号\n");
//                getchar();
//                getchar();
//
//				goto search;
//			}
//			}
//			goto search;
//		}
//		case 6://行权&做市商
//		{
//		Exec:system("clear");
//			int num_xingquan;
//			LOGDEMO("32.执行宣告录入请求\n");
//			LOGDEMO("33.执行宣告操作请求\n");
//			LOGDEMO("34.放弃行权\n");
//			LOGDEMO("35.询价录入请求\n");
//			LOGDEMO("36.做市商报价录入请求\n");
//			LOGDEMO("37.做市商报价撤销请求\n");
//            LOGDEMO("0.返回上一层");
//			LOGDEMO("请选择你需要的编码");
//			cin >> num_xingquan;
//			switch (num_xingquan)
//			{
//			case 32://执行宣告录入请求
//			{
//				sh.ReqExecOrderInsert(0);
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 33://执行宣告操作请求
//			{
//				sh.ReqExecOrderAction();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 34://放弃行权
//			{
//				sh.ReqExecOrderInsert(1);
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 35://询价录入请求
//			{
//				string g_chFrontMdaddr = getConfig("trade", "FrontMdAddr");
//				cout << "g_chFrontMdaddr = " << g_chFrontMdaddr << "\n" << endl;
//				CThostFtdcMdApi  *pUserMdApi =
//					CThostFtdcMdApi::CreateFtdcMdApi();
//				CSimpleMdHandler ash(pUserMdApi);
//				pUserMdApi->RegisterSpi(&ash);
//				pUserMdApi->RegisterFront(const_cast<char *>(g_chFrontMdaddr.c_str()));
//				pUserMdApi->Init();
//				//WaitForSingleObject(xinhao, INFINITE);
//                sem_wait(&sem2);
//				ash.SubscribeMarketData();//行情订阅询价请求
//				sh.ReqForQuoteInsert();//交易请求询价
//                getchar();
//                getchar();
//
//				pUserMdApi->Release();
//				break;
//			}
//			case 36://做市商报价录入请求
//			{
//				sh.ReqQuoteInsert();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 37://做市商报价撤销请求
//			{
//				sh.ReqQuoteAction();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 0:
//			{
//				goto loop;
//			}
//			default:
//				LOGDEMO("输入的编码有误，请重新输入\n");
//                getchar();
//                getchar();
//
//				//goto Exec;
//			}
//			goto Exec;
//		}
//		case 7://期货资金转账
//		{
//		futrue:system("clear");
//			int num_future;
//			LOGDEMO("38.期货发起银行资金转期货请求\n");
//			LOGDEMO("39.期货发起期货资金转银行请求\n");
//			LOGDEMO("0.返回上一层\n");
//			LOGDEMO("请输入你需要的操作序号：");
//			cin >> num_future;
//			switch (num_future)
//			{
//			case 38://期货发起银行资金转期货请求
//			{
//				sh.ReqFromBankToFutureByFuture();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 39://期货发起期货资金转银行请求
//			{
//				sh.ReqFromFutureToBankByFuture();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 0:
//			{
//				goto loop;
//			}
//			default:
//				LOGDEMO("输入的编码有误，请重新输入\n");
//                getchar();
//                getchar();
//
//				//goto futrue;
//			}
//			goto futrue;
//		}
//		case 8://铜期权测试
//		{
//		qiquan:system("clear");
//			int num_qiquan;
//			LOGDEMO("//////////////铜期权测试////////////\n");
//			LOGDEMO("1.期权自对冲录入请求\n");
//			LOGDEMO("2.期权自对冲操作请求\n");
//			LOGDEMO("3.请求查询期权自对冲\n");
//			LOGDEMO("0.返回上一层\n");
//			LOGDEMO("请选择你需要的编码:\n");
//			cin >> num_qiquan;
//			switch (num_qiquan)
//			{
//			case 1:
//			{
//				sh.ReqOptionSelfCloseInsert();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 2:
//			{
//				sh.ReqOptionSelfCloseAction();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 3:
//			{
//				sh.ReqQryOptionSelfClose();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 0:
//				goto loop;
//				break;
//			default:
//				LOGDEMO("输入的序号有误，请重新输入。\n\n");
//                getchar();
//                getchar();
//
//				goto qiquan;
//			}
//			goto qiquan;
//		}
//		case 9://版本6.3.13新加接口
//		{
//		NewVersion:
//			system("clear");
//			int num_Newversion;
//			LOGDEMO("新版本测试\n");
//			LOGDEMO("1.查询用户当前支持的认证模式(目前暂不启用)\n");
//			LOGDEMO("2.用户发出获取图形验证码请求(目前暂不启用)\n");
//			LOGDEMO("3.用户发出获取短信验证码请求(目前暂不启用)\n");
//			LOGDEMO("4.用户发出带有图片验证码的登陆请求(目前暂不启用)\n");
//			LOGDEMO("5.用户发出带有短信验证码的登陆请求(目前暂不启用)\n");
//			LOGDEMO("6.用户发出带有动态口令的登陆请求(目前暂不启用)\n");
//			LOGDEMO("0.返回上一层\n");
//			LOGDEMO("请选择你需要的编码:\n");
//			cin >> num_Newversion;
//			switch (num_Newversion)
//			{
//			case 1://查询用户当前支持的认证模式
//			{
//				sh.ReqUserAuthMethod();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 2://用户发出获取图形验证码请求
//			{
//				sh.ReqGenUserCaptcha();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 3://用户发出获取短信验证码请求
//			{
//				sh.ReqGenUserText();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 4://用户发出带有图片验证码的登陆请求
//			{
//				sh.ReqUserLoginWithCaptcha();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 5://用户发出带有短信验证码的登陆请求
//			{
//				sh.ReqUserLoginWithText();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 6://用户发出带有动态口令的登陆请求
//			{
//				sh.ReqUserLoginWithOTP();
//                getchar();
//                getchar();
//
//				break;
//			}
//			case 0:
//				goto loop;
//				break;
//			default:
//				LOGDEMO("输入的序号有误，请重新输入。\n\n");
//                getchar();
//                getchar();
//
//				goto NewVersion;
//			}
//			goto NewVersion;
//		}
//		case 0:
//			system("clear");
//			break;
//		case 100:
//			pUserApi->Release();
//			exit(-1);
//		}
//	}
//	return 0;
//}
//
