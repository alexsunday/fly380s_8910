/* Copyright (C) 2016 RDA Technologies Limited and/or its affiliates("RDA").
* All rights reserved.
*
* This software is supplied "AS IS" without any warranties.
* RDA assumes no responsibility or liability for the use of the software,
* conveys no license or title under any patent, copyright, or mask work
* right to the product. RDA reserves the right to make changes in the
* software without notification.  RDA also make no representation or
* warranty that such application will be suitable for the specified use
* without further testing or modification.
*/
#include "cfw.h"
#ifdef CFW_GPRS_SUPPORT
#include "stdio.h"
#include "osi_api.h"
#include "osi_log.h"
#include "at_response.h"
#include "at_cfw.h"
#include "at_engine.h"
#include "at_command.h"
#include "sockets.h"

extern void _dnsAddressToStr(const CFW_GPRS_PDPCONT_INFO_V2 *pdp_cont, char *str);

void AT_NET_CmdFunc_NetInfo(atCommand_t *pParam)
{
    char outString[512] = {
        0x00,
    };
    if (NULL == pParam)
    {
        RETURN_CME_ERR(pParam->engine, ERR_AT_CME_PARAM_INVALID);
        return;
    }
    switch (pParam->type)
    {
    case AT_CMD_READ:
    {
        struct netif *netinfo = NULL;

        if (netif_default != NULL)
        {
            sprintf(outString, "default netif: %c%c%d\r\n", netif_default->name[0], netif_default->name[1], netif_default->num);
            atCmdRespInfoText(pParam->engine, outString);
        }
        else
        {
            sprintf(outString, "default netif notset\r\n");
            atCmdRespInfoText(pParam->engine, outString);
        }
        NETIF_FOREACH(netinfo)
        {
            int cid = netinfo->sim_cid & 0x0f;
            int sim = (netinfo->sim_cid & 0xf0) >> 4;
            if (strstr(netinfo->name, "GP") != NULL)
            {
                sprintf(outString, "%c%c%d: sim:%d - cid:%d", netinfo->name[0], netinfo->name[1], netinfo->num, sim, cid);
                atCmdRespInfoText(pParam->engine, outString);
            }
            else
            {
                sprintf(outString, "%c%c%d:", netinfo->name[0], netinfo->name[1], netinfo->num);
                atCmdRespInfoText(pParam->engine, outString);
            }
            sprintf(outString, "  inet4 addr: %s", ipaddr_ntoa(&(netinfo->ip_addr)));
            atCmdRespInfoText(pParam->engine, outString);
#if LWIP_IPV6
            sprintf(outString, "  inet6 addr0: %s", ipaddr_ntoa(&(netinfo->ip6_addr[0])));
            atCmdRespInfoText(pParam->engine, outString);
            sprintf(outString, "  inet6 addr1: %s", ipaddr_ntoa(&(netinfo->ip6_addr[1])));
            atCmdRespInfoText(pParam->engine, outString);
#endif
            sprintf(outString, "  gw addr: %s", ipaddr_ntoa(&(netinfo->gw)));
            atCmdRespInfoText(pParam->engine, outString);
            sprintf(outString, "  mask addr: %s", ipaddr_ntoa(&(netinfo->netmask)));
            atCmdRespInfoText(pParam->engine, outString);
            const ip_addr_t *dns1 = NULL, *dns2 = NULL;
            if (cid > 0 && cid < 8 && sim < CFW_SIM_COUNT)
            {
                char dns_addr_str[200];
                CFW_GPRS_PDPCONT_INFO_V2 sPdpCont;
                CFW_GprsGetPdpCxtV2(cid, &sPdpCont, sim);
                _dnsAddressToStr(&sPdpCont, dns_addr_str);
                sprintf(outString, "  dns addr: %s", dns_addr_str);
                atCmdRespInfoText(pParam->engine, outString);
                dns1 = NULL;
                dns2 = NULL;
            }
            else if (netinfo->sim_cid == (0xf0 | 0x11))
            {
#ifdef WIFI_SUPPORT
                dns1 = wifi_dns_getserver(0);
                dns2 = wifi_dns_getserver(1);
#endif
            }
            else
            {
                dns1 = NULL;
                dns2 = NULL;
            }
            if (dns1 != NULL)
            {
                sprintf(outString, "  dns addr: %s", ipaddr_ntoa(dns1));
                atCmdRespInfoText(pParam->engine, outString);
            }
            if (dns2 != NULL)
            {
                sprintf(outString, "  dns2 addr: %s", ipaddr_ntoa(dns2));
                atCmdRespInfoText(pParam->engine, outString);
            }

            //Test data count API
            uint32_t datacount = 0;
            char *propmtString[3] = {"LWIP", "PPP", "RNDIS"};
            int i = 0;
            for (i = 1; i <= RNDIS_DATA; i++)
            {
                CFW_get_Netif_dataCount(sim, cid, UPLOAD, i, &datacount);
                sprintf(outString, "  UPLOAD %s : %ld", propmtString[i - 1], datacount);
                atCmdRespInfoText(pParam->engine, outString);

                CFW_get_Netif_dataCount(sim, cid, DOWNLOAD, i, &datacount);
                sprintf(outString, "  DOWNLOAD %s : %ld", propmtString[i - 1], datacount);
                atCmdRespInfoText(pParam->engine, outString);
            }
        }
        atCmdRespOK(pParam->engine);
        break;
    }
#if 0	
    case AT_CMD_SET:
    {
        UINT8 uParamCount = 0;
        INT32 iRet = ERR_SUCCESS;
        UINT8 ifname[6];
        UINT16 uSize = 6;
        iRet = AT_Util_GetParaCount(pParam->pPara, &uParamCount);
        if ((iRet != ERR_SUCCESS) || (uParamCount != 1))
        {
            COS_LOGI(0x100040b9, "AT_NET_CmdFunc_NetInfo, parameters error or parameter number not satisfied\n");
            at_CmdRespCmeError(pParam->engine, ERR_AT_CME_PARAM_INVALID);
            return;
        }
        iRet = AT_Util_GetParaWithRule(pParam->pPara, 0, AT_UTIL_PARA_TYPE_STRING, ifname, &uSize);
        COS_LOGXI(COS_LOGPAR_S, 0x100040ba, "AT_NET_CmdFunc_NetInfo... ... ifname= %s", ifname);
        if (iRet != ERR_SUCCESS)
        {
            COS_LOGI(0x100040bb, "AT_NET_CmdFunc_NetInfo, get parameter error\n");
            at_CmdRespCmeError(pParam->engine, ERR_AT_CME_PARAM_INVALID);
            return;
        }
        else
        {
            struct netif *netif = netif_find(ifname);
            if (netif != NULL)
            {
                if (netif_default != netif)
                {
                    netif_set_default(netif);
                    int cid = netif->sim_cid & 0x0f;
                    int sim = (netif->sim_cid & 0xf0) >> 4;
                    if (cid > 0 && cid < 8 && sim < CFW_SIM_COUNT)
                    {
                        UINT32 *server = getDNSServer(cid, sim);
                        ip_addr_t dns_server = IPADDR4_INIT(server[0]);
                        ip_addr_t dns_server2 = IPADDR4_INIT(server[1]);
                        dns_setserver(0, &dns_server);
                        dns_setserver(1, &dns_server2);
                    }
                    else if (netif->sim_cid == (0xf0 | 0x11))
                    {
#ifdef WIFI_SUPPORT
                        dns_setserver(0, wifi_dns_getserver(0));
                        dns_setserver(1, wifi_dns_getserver(1));
#endif
                    }
                }
                at_CmdRespOK(pParam->engine);
            }
            else
            {
                COS_LOGI(0x100040bc, "NetInfo set ERROR");
                at_CmdRespCmeError(pParam->engine, ERR_AT_CME_PARAM_INVALID);
            }
        }
    }
    break;
#endif
    default:
        OSI_LOGI(0x100040bd, "NetInfo ERROR");
        RETURN_CME_ERR(pParam->engine, ERR_AT_CME_PARAM_INVALID);
        break;
    }
}

#endif
