#pragma once
#include <string>
#include "servant/Application.h"
#include "Java2RoomProto.h"
#include "RoomServant.h"

using namespace JFGame;

class ChangePositionClubRequest
{
    friend class ChangePositionClubRespons;
public:
    ChangePositionClubRequest() {}
    ChangePositionClubRequest(const std::string &json)
    {
        this->Deserialize(json);
    }
    template <typename Writer>
    void Serialize(Writer &writer) const
    {
        writer.StartObject();
        SERIALIZE_MEMBER(writer, iType);
        SERIALIZE_MEMBER(writer, sUid);
        SERIALIZE_MEMBER(writer, sClubID);
        SERIALIZE_MEMBER(writer, sTargetUid);
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
            SET_DOC_MEMBER(d, iType);
            SET_DOC_MEMBER(d, sUid);
            SET_DOC_MEMBER(d, sClubID);
            SET_DOC_MEMBER(d, sTargetUid);
        }
        catch (const std::exception &e)
        {
            std::string errInfo = ::toString(__FILE__, ":", __LINE__, ":ChangePositionClubRequest decode error!");
            throw logic_error(errInfo);
        }
    }

    static tars::Int32 handler(const vector<tars::Char> &reqBuf, const map<std::string, std::string> &extraInfo, vector<tars::Char> &rspBuf)
    {
        return 0;
    }

private:
    CInteger _iType;        // 操作类型 1: 会长， 2: 管理员, 3: 成员
    CString _sUid;          // 会长UID     
    CString _sClubID;       // 俱乐部ID
    CString _sTargetUid;    // 成员UID  
};
class ChangePositionClubRespons
{
public:
    ChangePositionClubRespons() {}
    ChangePositionClubRespons(const string &json)
    {
        this->Deserialize(json);
    }
    template <typename Writer>
    void Serialize(Writer &writer) const
    {
        writer.StartObject();
        SERIALIZE_MEMBER(writer, iType);
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
        SET_DOC_MEMBER(d, iType);
    }

    static tars::Int32 handler(const vector<tars::Char> &reqBuf, const map<std::string, std::string> &extraInfo, vector<tars::Char> &rspBuf)
    {
        FUNC_ENTRY("");
        __TRY__

        // STEP1 解码
        ChangePositionClubRequest request;
        decode(reqBuf, request);

        // STEP2 具体业务处理
        int64_t resultCode = RESULT_CODE_SUCCESS;
       
        Club::InnerClubChangePositionReq req;
        Club::InnerClubChangePositionResp resp;
        req.clubId = S2L(request._sClubID);
        req.uId = S2L(request._sUid);
        req.memberId = S2L(request._sTargetUid);
        req.position = request._iType;
        int iRet = g_app.getOuterFactoryPtr()->getSocialServerPrx(req.clubId)->InnerClubChangePosition(req, resp);
        if (iRet != 0)
        {
            ROLLLOG_ERROR << "InnerClubChangePosition failed, iRet:" << iRet << ", request._sClubID:" << req.clubId << endl;
            resultCode = RESULT_CODE_FAIL;
        }

        // STEP3 填充数据
        encode(resultCode, request, rspBuf);
        
        __CATCH__
        FUNC_EXIT("", 0);
        return 0;
    }

private:

    static void encode(int64_t resultCode, ChangePositionClubRequest &request, vector<tars::Char> &rspBuf)
    {
        ChangePositionClubRespons  response;
        response._iType.assign(request._iType);

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

private:
    CInteger _iType;        // 操作类型 1: 会长， 2: 管理员, 3: 成员
};
