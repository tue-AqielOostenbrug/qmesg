#include <CUnit/Basic.h>
#include <fcntl.h>
#include <unistd.h>
#include "../src/linux/client/c2s_irc.c"
#include <string.h>
#include <termios.h>

// Global variables
int write_fd = -1;
int read_fd = -1;

/** The IRC suite initialization function.
 * Returns zero on success, non-zero otherwise.
 */
int init_irc_suite(void)
{
   printf("Intializing Suite:\n");
   printf("> Generating file descriptors\n");
   write_fd = open("temp", O_WRONLY | O_CREAT, 777);
   printf("- write_fd = %d\n", write_fd);
   read_fd = open("temp", O_RDONLY, 777);
   printf("- read_fd = %d\n", read_fd);
   if(write_fd == -1 || read_fd == -1) return 1;
   return 0;
}

/** The IRC suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_irc_suite(void)
{
   printf("\n> Cleaning IRC Suite\n");
   if(close(write_fd) || close(read_fd) || unlink("temp")) return 1;
   return 0;
}

/** Simple dummy test.
 */
void test_dummy(void)
{
   CU_ASSERT(0 == 0);
}

/** PASS test.
 */
void test_pass(void)
{
   // Prepare variables
   char res[11] = "";

   // Proper
   CU_ASSERT_EQUAL(pass(write_fd, "psw"), 0);
   CU_ASSERT_NOT_EQUAL(read(read_fd, res, 10), -1);
   CU_ASSERT_STRING_EQUAL("PASS psw\r\n",res);

   //PASS test with bad fd.
   CU_ASSERT_EQUAL(pass(-1, "psw"), 1);

   // PASS test with bad psw.
   CU_ASSERT_EQUAL(pass(write_fd, NULL), 2);

   // PASS test with max length (512 - 5 - 2 = 505).
   CU_ASSERT_EQUAL(
      pass
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. V"
      ),
      0
   );
   char max[512] = "";
   CU_ASSERT_NOT_EQUAL(read(read_fd, max, 512), -1);
   CU_ASSERT_STRING_EQUAL(
      "PASS Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. V\r\n",
      max
   );

   // PASS test with max +1 length
   CU_ASSERT_EQUAL(
      pass
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vx"
      ),
      3
   );
}

/** NICK test.
 */
void test_nick(void)
{
   // Prepare variables
   char res[11] = "";

   // Proper
   CU_ASSERT_EQUAL(nick(write_fd, "nck"), 0);
   CU_ASSERT_NOT_EQUAL(read(read_fd, res, 10), -1);
   CU_ASSERT_STRING_EQUAL("NICK nck\r\n", res);

   // NICK test with bad fd.
   CU_ASSERT_EQUAL(nick(-1, "nck"), 1);

   // NICK test with bad nck.
   CU_ASSERT_EQUAL(nick(write_fd, NULL), 2);

   // NICK test with max length (512 - 5 - 2 = 505).
   CU_ASSERT_EQUAL(
      nick
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. V"
      ),
      0
   );
   char max[512] = "";
   CU_ASSERT_NOT_EQUAL(read(read_fd, max, 512), -1);
   CU_ASSERT_STRING_EQUAL(
      "NICK Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. V\r\n",
      max
   );

   // NICK test with max + 1 length
   CU_ASSERT_EQUAL(
      nick
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vx"
      ),
      3
   );
}

/** USER test.
 */
void test_user(void)
{
   // Prepare variables
   char res[20] = "";

   // Proper
   CU_ASSERT_EQUAL(user(write_fd, "usn", "rln"), 0);
   CU_ASSERT_NOT_EQUAL(read(read_fd, res, 19), -1);
   CU_ASSERT_STRING_EQUAL("USER usn 0 * :rln\r\n", res);

   // USER test with bad fd.
   CU_ASSERT_EQUAL(user(-1, "usn", "rln"), 1);

   // USER test with bad usn.
   CU_ASSERT_EQUAL(user(write_fd, NULL, "rln"), 2);

   // USER test with bad rln.
   CU_ASSERT_EQUAL(user(write_fd, "usn", NULL), 3);

   // USER test with max length (512 - 5 - 6 - 2 = 498 + 1).
   CU_ASSERT_EQUAL(
      user
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dap",
         "i"
      ),
      0
   );
   char max[512] = "";
   CU_ASSERT_NOT_EQUAL(read(read_fd, max, 512), -1);
   CU_ASSERT_STRING_EQUAL(
      "USER Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dap 0 * :i\r\n",
      max
   );

   // USER test with max + 1 arg length.
   CU_ASSERT_EQUAL(
      user
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapx",
         "i"
      ),
      4
   );
}

/** PING test.
 */
void test_ping(void)
{
   // Prepare variables
   char res[11] = "";

   // Proper
   CU_ASSERT_EQUAL(ping(write_fd, "tkn"), 0);
   read(read_fd, res, 10);
   CU_ASSERT_STRING_EQUAL("PING tkn\r\n", res);
   
   // PING test with bad fd.
   CU_ASSERT_EQUAL(ping(-1, "tkn"), 1);
   
   // PING test with bad tkn.
   CU_ASSERT_EQUAL(ping(write_fd, NULL), 2);

   // PING test with max length (512 - 5 - 2 = 505).
   CU_ASSERT_EQUAL(
      ping
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. V"
      ),
      0
   );
   char max[512] = "";
   CU_ASSERT_NOT_EQUAL(read(read_fd, max, 512), -1);
   CU_ASSERT_STRING_EQUAL(
      "PING Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. V\r\n",
      max
   );

   // PING test with max + 1length
   CU_ASSERT_EQUAL(
      ping
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vx"
      ),
      3
   );
}

/** PONG test.
 */
void test_pong(void)
{
   // Prepare variables
   char res[11] = "";

   // Proper
   CU_ASSERT_EQUAL(pong(write_fd, "tkn"), 0);
   read(read_fd, res, 10);
   CU_ASSERT_STRING_EQUAL("PONG tkn\r\n", res);
   
   // PONG test with bad fd.
   CU_ASSERT_EQUAL(pong(-1, "tkn"), 1);
   
   // PONG test with bad tkn.
   CU_ASSERT_EQUAL(pong(write_fd, NULL), 2);

   // PONG test with max length (512 - 5 - 2 = 505).
   CU_ASSERT_EQUAL(
      pong
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. V"
      ),
      0
   );
   char max[512] = "";
   CU_ASSERT_NOT_EQUAL(read(read_fd, max, 512), -1);
   CU_ASSERT_STRING_EQUAL(
      "PONG Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. V\r\n",
      max
   );

   // PONG test with max + 1 length
   CU_ASSERT_EQUAL(
      pong
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vx"
      ),
      3
   );
}

/** JOIN test.
 */
void test_join(void)
{
   // Prepare variables
   char res[12] = "";

   // Proper
   CU_ASSERT_EQUAL(join(write_fd, "cnl"), 0);
   read(read_fd, res, 11);
   CU_ASSERT_STRING_EQUAL("JOIN #cnl\r\n", res);

   // JOIN test with bad fd.
   CU_ASSERT_EQUAL(join(-1, "cnl"), 1);
   
   // JOIN test with bad cnl.
   CU_ASSERT_EQUAL(join(write_fd, NULL), 2);
   
   // JOIN test with max length (512 - 6 - 2 = 504).
   CU_ASSERT_EQUAL(
      join
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. "
      ),
      0
   );
   char max[512] = "";
   CU_ASSERT_NOT_EQUAL(read(read_fd, max, 512), -1);
   CU_ASSERT_STRING_EQUAL(
      "JOIN #Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. \r\n",
      max
   );

   // JOIN test with max + 1 length
   CU_ASSERT_EQUAL(
      join
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. x"
      ),
      3
   );
}

/** QUIT test.
 */
void test_quit(void)
{
   char res[7];
   res[6] = '\0';
   CU_ASSERT_EQUAL(quit(write_fd), 0);
   read(read_fd, res, 6);
   CU_ASSERT_STRING_EQUAL("QUIT\r\n", res);
}

/** PRIVMSG test.
 */
void test_privmsg(void)
{
   // Prepare variables
   char res[24] = "";

   // Proper
   CU_ASSERT_EQUAL(privmsg(write_fd, "cnl", "msg"), 0);
   read(read_fd, res, 23);
   CU_ASSERT_STRING_EQUAL("PRIVMSG #cnl msg\r\n", res);

   // PRIVMSG test with bad fd.
   CU_ASSERT_EQUAL(privmsg(-1, "cnl", "msg"), 1);

   // PRIVMSG test with bad cnl.
   CU_ASSERT_EQUAL(privmsg(write_fd, NULL, "msg"), 2);

   // PRIVMSG test with bad msg.
   CU_ASSERT_EQUAL(privmsg(write_fd, "cnl", NULL), 3);

   // PRIVMSG test with max length (512 - 9 - 1 - 2 = 499 + 1).
   CU_ASSERT_EQUAL(
      privmsg
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapi",
         "b"
      ),
      0
   );
   char max[512] = "";
   CU_ASSERT_NOT_EQUAL(read(read_fd, max, 512), -1);
   CU_ASSERT_STRING_EQUAL(
      "PRIVMSG #Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapi b\r\n",
      max
   );

   // PRIVMSG test with max + 1 arg length.
   CU_ASSERT_EQUAL(
      privmsg
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapix",
         "b"
      ),
      4
   );
}

/** LIST test.
 */
void test_list(void)
{
   char res[7];
   res[6] = '\0';
   CU_ASSERT_EQUAL(list(write_fd), 0);
   read(read_fd, res, 6);
   CU_ASSERT_STRING_EQUAL("LIST\r\n", res);
}

/** TOPIC test.
 */
void test_topic(void)
{
   // Prepare variables
   char res[13] = "";

   // Proper
   CU_ASSERT_EQUAL(topic(write_fd, "cnl"), 0);
   read(read_fd, res, 12);
   CU_ASSERT_STRING_EQUAL("TOPIC #cnl\r\n", res);

   // TOPIC test with bad fd.
   CU_ASSERT_EQUAL(topic(-1, "cnl"), 1);
   
   // TOPIC test with bad cnl.
   CU_ASSERT_EQUAL(topic(write_fd, NULL), 2);
   
   // TOPIC test with max length (512 - 7 - 2 = 503).
   CU_ASSERT_EQUAL(
      topic
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus."
      ),
      0
   );
   char max[512] = "";
   CU_ASSERT_NOT_EQUAL(read(read_fd, max, 512), -1);
   CU_ASSERT_STRING_EQUAL(
      "TOPIC #Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus.\r\n",
      max
   );

   // TOPIC test with max + 1 length
   CU_ASSERT_EQUAL(
      topic
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus.x"
      ),
      3
   );
}

/** PART test.
 */
void test_part(void)
{
   // Prepare variables
   char res[12] = "";

   // Proper
   CU_ASSERT_EQUAL(part(write_fd, "cnl"), 0);
   read(read_fd, res, 11);
   CU_ASSERT_STRING_EQUAL("PART #cnl\r\n", res);

   // PART test with bad fd.
   CU_ASSERT_EQUAL(part(-1, "cnl"), 1);
   
   // PART test with bad cnl.
   CU_ASSERT_EQUAL(part(write_fd, NULL), 2);
   
   // JOIN test with bad cnl.
   CU_ASSERT_EQUAL(join(write_fd, NULL), 2);
   
   // PART test with max length (512 - 6 - 2 = 504).
   CU_ASSERT_EQUAL(
      part
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. "
      ),
      0
   );
   char max[512] = "";
   CU_ASSERT_NOT_EQUAL(read(read_fd, max, 512), -1);
   CU_ASSERT_STRING_EQUAL(
      "PART #Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. \r\n",
      max
   );

   // PART test with max + 1 length
   CU_ASSERT_EQUAL(
      part
      (
         write_fd,
         "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. x"
      ),
      3
   );
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite ircSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   ircSuite = CU_add_suite("IRC Suite", init_irc_suite, clean_irc_suite);
   if (NULL == ircSuite) {
      printf("IRC Suite couldn't be added!\n");
      CU_cleanup_registry();
      return CU_get_error();
   }

   // Add IRC tests
   if (NULL == CU_add_test(ircSuite, "Dummy test\n", test_dummy))
   {
      printf("An Dummy test has failed\n");
      CU_cleanup_registry();
      return CU_get_error();
   }
   if (NULL == CU_add_test(ircSuite, "PASS test\n", test_pass))
   {
      printf("An PASS test has failed\n");
      CU_cleanup_registry();
      return CU_get_error();
   }
   if (NULL == CU_add_test(ircSuite, "NICK test\n", test_nick))
   {
      printf("An NICK test has failed\n");
      CU_cleanup_registry();
      return CU_get_error();
   }
   if (NULL == CU_add_test(ircSuite, "USER test\n", test_user))
   {
      printf("An USER test has failed\n");
      CU_cleanup_registry();
      return CU_get_error();
   }
   if (NULL == CU_add_test(ircSuite, "PING test\n", test_ping))
   {
      printf("An PING test has failed\n");
      CU_cleanup_registry();
      return CU_get_error();
   }
   if (NULL == CU_add_test(ircSuite, "PONG test\n", test_pong))
   {
      printf("An PONG test has failed\n");
      CU_cleanup_registry();
      return CU_get_error();
   }
   if (NULL == CU_add_test(ircSuite, "JOIN test\n", test_join))
   {
      printf("An JOIN test has failed\n");
      CU_cleanup_registry();
      return CU_get_error();
   }
   if (NULL == CU_add_test(ircSuite, "QUIT test\n", test_quit))
   {
      printf("An QUIT test has failed\n");
      CU_cleanup_registry();
      return CU_get_error();
   }
   if (NULL == CU_add_test(ircSuite, "PRIVMSG test\n", test_privmsg))
   {
      printf("An PRIVMSG test has failed\n");
      CU_cleanup_registry();
      return CU_get_error();
   }
   if (NULL == CU_add_test(ircSuite, "LIST test\n", test_quit))
   {
      printf("An LIST test has failed\n");
      CU_cleanup_registry();
      return CU_get_error();
   }
   if (NULL == CU_add_test(ircSuite, "TOPIC test\n", test_topic))
   {
      printf("An TOPIC test has failed\n");
      CU_cleanup_registry();
      return CU_get_error();
   }
   if (NULL == CU_add_test(ircSuite, "PART test\n", test_part))
   {
      printf("An PART test has failed\n");
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
