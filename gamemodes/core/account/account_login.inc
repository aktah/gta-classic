#include <YSI_Coding\y_hooks>

static 
    PasswordAttempts[MAX_PLAYERS],
    bool:Player_LoginStatus[MAX_PLAYERS];

hook OnPlayerConnect(playerid) {
    PasswordAttempts[playerid]      = 0;
    Player_LoginStatus[playerid]    = false;
    return 1;
}

Account_PromptLogin(playerid) {
    static 
        const caption[] = 
        "\
            ยินดีต้อนรับเข้าสู่ "SERVER_NAME"\
            กรุณาใส่รหัสผ่านเพื่อล็อคอิน\
        ";
    Dialog_ShowCallback(playerid,
        using public Account_Login<iiiis>, 
        DIALOG_STYLE_PASSWORD, 
        SERVER_NAME, 
        caption, 
        "ล็อคอิน",
        "ออกจากเกม");
    return 1;
}

forward Account_Login(playerid, dialogid, response, listitem, string:inputtext[]);
public Account_Login(playerid, dialogid, response, listitem, string:inputtext[]) {
    if (response) {
        new
            query[250],
            buffer[129];

        WP_Hash(buffer, sizeof(buffer), inputtext);

        mysql_format(MySQL_GetHandle(), query, sizeof(query), 
        "\
            SELECT * FROM `players` WHERE `username` = '%e' AND `password` = '%s' LIMIT 1\
        ", 
            Player_GetName(playerid), 
            buffer
        );
        mysql_tquery(MySQL_GetHandle(), query, "Account_LoginAttemps", "d", playerid);
    }
    else {
        Kick(playerid);
    }
    return 1;
}

forward Account_LoginAttemps(playerid);
public Account_LoginAttemps(playerid) {

    new 
        rows;

    cache_get_row_count(rows);

    if (rows)
    {
        PasswordAttempts[playerid] = 0;
        CallLocalFunction("OnPlayerLogin", "d", playerid);
    }
    else
    {
        PasswordAttempts[playerid]++;
        if (PasswordAttempts[playerid] >= 3)
        {
            SendClientMessage(playerid, COLOR_RED, "รหัสผ่านไม่ถูกต้องครบ 3/3 ระบบจึงเตะคุณออกจากเซิร์ฟเวอร์");
            SendClientMessage(playerid, COLOR_RED, "ช่วยเหลือ: /q ในการออกจากหน้าต่างเกม");
            Kick_Message(playerid);
        }
        else
        {
            va_SendClientMessage(playerid, COLOR_RED, "รหัสผ่านไม่ถูกต้อง %d/3", PasswordAttempts[playerid]);
            SendClientMessage(playerid, COLOR_RED, "หากคุณใส่รหัสผ่านไม่ถูกต้องครบ 3 ครั้ง ระบบจะทำการเตะคุณออกจากเซิร์ฟเวอร์");
            Account_PromptLogin(playerid);
        }
    }
    return 1;
}

hook OnPlayerLogin(playerid) {
    new 
        date[11],
        time[9],
        ip[16];

    cache_get_value_name(0, "date_reg", date, 11);
    cache_get_value_name(0, "time_reg", time, 9);
    cache_get_value_name(0, "ip", ip, 16);

    Player_SetAccountDate(playerid, date);
    Player_SetAccountTime(playerid, time);
    Player_SetAccountIP(playerid, ip);

    ClearPlayerChat(playerid, 20);
    SendClientMessage(playerid, COLOR_YELLOW, "ยินดีต้อนรับเข้าสู่ "SERVER_NAME" เวอร์ชั่น: "SERVER_VERSION"");

    Player_SetLoginStatus(playerid, true);
    return 1;
}

Player_IsLoggedIn(playerid) {
    return Player_LoginStatus[playerid];
}

Player_SetLoginStatus(playerid, bool:status) {
    return Player_LoginStatus[playerid] = status;
}