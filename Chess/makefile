CC = gcc
OBJS = ChessErrorHandler.o ChessGameCommon.o ChessCmdParser.o ArrayList.o ChessGameMove.o ChessGame.o GameSettings.o \
LoadGame.o SaveGame.o UI_Widget.o UI_Button.o UI_Auxiliary.o UI_Window.o UI_WindowController.o \
UI_MainWindow.o UI_MainWindowController.o UI_SettingsWindow.o UI_SettingsWindowController.o \
UI_LoadGameWindow.o UI_LoadGameWindowController.o UI_GameWindow.o UI_GameWindowController.o main.o
 
EXEC = chessprog
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

$(EXEC) : $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
ArrayList.o: ChessGameCommon.h ChessErrorHandler.h ArrayList.h ArrayList.c 
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessCmdParser.o: ChessErrorHandler.h ChessCmdParser.h ChessCmdParser.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessErrorHandler.o: ChessErrorHandler.c ChessErrorHandler.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessGameCommon.o: ChessGameCommon.h ChessGameCommon.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessGameMove.o: ChessErrorHandler.h ChessGameCommon.h ArrayList.h ChessGameMove.h ChessGameMove.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
ChessGame.o: ChessErrorHandler.h ChessGameCommon.h ArrayList.h ChessGameMove.h ChessGame.h ChessGame.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
GameSettings.o: ChessErrorHandler.h ChessGameCommon.h ArrayList.h ChessGameMove.h ChessGame.h GameSettings.h GameSettings.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
LoadGame.o: ChessErrorHandler.h GameSettings.h LoadGame.h LoadGame.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SaveGame.o: ChessErrorHandler.h GameSettings.h SaveGame.h SaveGame.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c 
UI_Widget.o: UI_Widget.h ChessErrorHandler.h UI_Widget.c 
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
UI_Button.o: UI_Widget.h ChessErrorHandler.h UI_Button.h UI_Button.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
UI_Auxiliary.o: UI_Widget.h ChessErrorHandler.h UI_Button.h ChessGame.h UI_Auxiliary.h UI_Auxiliary.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
UI_Window.o: UI_Auxiliary.h UI_Widget.h ChessErrorHandler.h UI_Button.h UI_Window.h UI_Window.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
UI_WindowController.o: ChessErrorHandler.h ChessGame.h UI_Window.h UI_WindowController.h UI_WindowController.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
UI_MainWindow.o: UI_Auxiliary.h UI_Widget.h ChessErrorHandler.h UI_Button.h UI_Window.h UI_MainWindow.h UI_MainWindow.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
UI_MainWindowController.o: ChessErrorHandler.h ChessGame.h UI_Window.h UI_WindowController.h UI_LoadGameWindowController.h UI_SettingsWindowController.h UI_MainWindow.h UI_MainWindowController.h UI_MainWindowController.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
UI_SettingsWindow.o: UI_Auxiliary.h UI_Widget.h ChessErrorHandler.h UI_Button.h UI_Window.h UI_SettingsWindow.h UI_SettingsWindow.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
UI_SettingsWindowController.o: ChessErrorHandler.h ChessGame.h GameSettings.h UI_Window.h UI_WindowController.h UI_GameWindowController.h UI_SettingsWindowController.h UI_MainWindowController.h UI_SettingsWindow.h UI_SettingsWindowController.h UI_SettingsWindowController.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
UI_LoadGameWindow.o: UI_Auxiliary.h UI_Widget.h ChessErrorHandler.h UI_Button.h UI_Window.h UI_LoadGameWindow.h UI_LoadGameWindow.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
UI_LoadGameWindowController.o: ChessErrorHandler.h ChessGame.h GameSettings.h SaveGame.h LoadGame.h UI_Window.h UI_WindowController.h UI_LoadGameWindowController.h UI_MainWindowController.h UI_SettingsWindowController.h UI_LoadGameWindow.h UI_LoadGameWindowController.h UI_LoadGameWindowController.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
UI_GameWindow.o: UI_Auxiliary.h UI_Widget.h ChessErrorHandler.h UI_Button.h ChessGame.h UI_Window.h UI_GameWindow.h UI_GameWindow.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
UI_GameWindowController.o: ChessErrorHandler.h GameSettings.h UI_Window.h UI_WindowController.h UI_LoadGameWindowController.h UI_MainWindowController.h UI_GameWindow.h UI_GameWindowController.h UI_GameWindowController.c
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
main.o: main.c UI_Auxiliary.h UI_Window.h UI_WindowController.h UI_MainWindowController.h ChessErrorHandler.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
clean:
	rm -f *.o $(EXEC)
