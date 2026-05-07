#pragma once
#include <string>
#include "servant/Application.h"
#include "Java2RoomProto.h"
#include "RoomServant.h"

using namespace JFGame;

class ChangeClubMerchantLevelRequest
{
    friend class ChangeClubMerchantLevelRespons;
public:
    ChangeClubMerchantLevelRequest() {}
    ChangeClubMerchantLevelRequest(const std::string &json)
    {
        this->Deserialize(json);
    }
    template <typename Writer>
    void Serialize(Writer &writer) const
    {
        writer.StartObject();

        SERIALIZE_MEMBER(writer, sClubID);
        SERIALIZE_MEMBER(writer, sLevel);
        writer.EndObject();
    }

    void toString(std::string &json)
    {
        StringBuffer sb;
        Writer<StringBuffer> writer(sb);
        Serialize(writer);
        json = sb.GetString();
    }

    void Deserialize(const string &json)
    {
        try
        {
            Document d;
            if (d.Parse(json.c_str()).HasParseError())
            {
                throw logic_error("parse json error. raw data : " + json);
            }

            SET_DOC_MEMBER(d, sClubID);
            SET_DOC_MEMBER(d, sLevel);
        }
        catch (const std::exception &e)
        {
            std::string errInfo = ::toString(__FILE__, ":", __LINE__, ":ChangeClubMerchantLevelRequest decode error!");
            throw logic_error(errInfo);
        }
    }

    static tars::Int32 handler(const vector<tars::Char> &reqBuf, const map<std::string, std::string> &extraInfo, vector<tars::Char> &rspBuf)
    {
        return 0;
    }

private:  
    CString _sClubID;       // 俱乐部ID
    CString _sLevel;        // 等级
};
class ChangeClubMerchantLevelRespons
{
public:
    ChangeClubMerchantLevelRespons() {}
    ChangeClubMerchantLevelRespons(const string &json)
    {
        this->Deserialize(json);
    }
    template <typename Writer>
    void Serialize(Writer &writer) const
    {
        writer.StartObject();
        writer.EndObject();
    }

    void toString(std::string &json)
    {
        StringBuffer sb;
        Writer<StringBuffer> writer(sb);
        Serialize(writer);
        json = sb.GetString();
    }

    void Deserialize(const string &json)
    {
        Document d;
        if (d.Parse(json.c_str()).HasParseError())
        {
            throw logic_error("parse json error. raw data : " + json);
        }
    }

    static tars::Int32 handler(const vector<tars::Char> &reqBuf, const map<std::string, std::string> &extraInfo, vector<tars::Char> &rspBuf)
    {
        FUNC_ENTRY("");
        __TRY__

        // STEP1 解码
        ChangeClubMerchantLevelRequest request;
        decode(reqBuf, request);

        // STEP2 具体业务处理
        int64_t resultCode = RESULT_CODE_SUCCESS;
       
        Club::UpdateClubInfoMerchantLevelReq req;
        Club::UpdateClubInfoMerchantLevelResp resp;
        req.cId = S2L(request._sClubID);
        req.level = S2L(request._sLevel);
        int iRet = g_app.getOuterFactoryPtr()->getSocialServerPrx(req.cId)->UpdateClubInfoMerchantLevel(req, resp);
        if (iRet != 0)
        {
            ROLLLOG_ERROR << "UpdateClubInfoMerchantLevel failed, iRet:" << iRet << ", request._sClubID:" << req.cId << endl;
            resultCode = RESULT_CODE_FAIL;
        }

        // STEP3 填充数据
        encode(resultCode, request, rspBuf);
        
        __CATCH__
        FUNC_EXIT("", 0);
        return 0;
    }

private:

    static void encode(int64_t resultCode, ChangeClubMerchantLevelRequest &request, vector<tars::Char> &rspBuf)
    {
        ChangeClubMerchantLevelRespons  response;

        // resultData是数组
        std::string json;
        response.toString(json);
        std::string resultData = "[" + json + "]";

        int64_t totalItems = 1;  //总条数
        int64_t totalPages = 1;  //总页数
        GMResponse rsp(resultCode, "", resultData, totalItems, totalPages);
        std::string resultJson;
        rsp.toString(resultJson);
        rspBuf.assign(resultJson.begin(), resultJson.end());
    }
};
