#property copyright "wendelmaques@gmail.com"
#property link      "11/10/2011"

#import "mql_socket.dll"
int mql_connect(string host, int port);
string mql_process(string buffer);
void mql_disconect();
int mql_connection_status;

int CMD_MARKET_INFO = 1;
int CMD_PRICE_DATA = 2;

void send_market_info(){
   string buffer = CMD_MARKET_INFO+":"+Symbol();
          buffer = buffer + ","+MarketInfo(Symbol(),MODE_POINT);
          buffer = buffer + ","+DoubleToStr(MarketInfo(Symbol(),MODE_DIGITS),0);
          buffer = buffer + ","+AccountNumber();
          buffer = buffer + "\n";
          execute_send_receive( mql_process( buffer ) );
}
int init(){
   string ip = "127.0.0.1";
   mql_connection_status = mql_connect(ip,20007);
   send_market_info();
   return(mql_connection_status);
}
int deinit(){
   mql_disconect();
   return(0);
}
bool got_error( string buffer ){
   if( StringSubstr(buffer,0,4) == "erro" )
      return(true);
   else return(false);
}
int start(){
   if( mql_connection_status != 0 ){
      log("Error, disconected!. Reconnecting!");
      init();
      return(-1);
   }
   string bid    = DoubleToStr(MarketInfo(Symbol(),MODE_BID),Digits);
   string ask    = DoubleToStr(MarketInfo(Symbol(),MODE_ASK),Digits);
   int    spread = MarketInfo(Symbol(),MODE_SPREAD);
   int    time   = MarketInfo(Symbol(),MODE_TIME);
   // warning! "string buffer" must be the same or you get a empety string!
   string buffer = CMD_PRICE_DATA+":"+bid+","+ask+","+spread+","+time+"\n";
   execute_send_receive( mql_process( buffer ) );
   return(0);
}
void execute_send_receive( string cmd ){
   if(StrToInteger(cmd)==0) return;
   cmd = StringSubstr(cmd,0,StringLen(cmd)-1);
   int p = StringFind(cmd,":",0);
   if( p == -1 ) return;
   int id = StrToInteger(StringSubstr(cmd,0,p));
   string args = StringSubstr(cmd,p+1);
   if( got_error(cmd) ){
      mql_connection_status = -1;
      return;
   }
   log( "RECEIVED COMMAND: id=" + id + ", args="+args+"" );
}
string log_buffer;
void log(string str){
   log_buffer = Hour()+":"+Minute()+":"+Seconds()+" - "+str+"\n"+log_buffer;
   Comment( StringSubstr(log_buffer,0,256) );
}

