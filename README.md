# TicTacToe
Правила на играта:

Играта се играе от двама играчи, които се редуват да поставят символи 0 или Х в поле 3х3.
Играта стартира с натискането на бутон "Старт" като след това той става неактивен докато играта не приключи
с последния ход на играч или победа. Първият играч играе с кръгове, а вторият с хиксове. По всяко време играта
може да бъде стартирана отново, както и да бъде видян екранът с информация. При това три еднакви символа в права линия
единия от играчите печели. Играта приключва при три еднакви символа в права илия или когато полето се запълни.

Изисквания към играта:

1. Бутони: 
1.1. Бутоните имат три състояния - натиснато, ненатиснато и неактивно. И трите състояния се визуализират с различна картинка.
При натискане на бутон, състоянието (картинката) се променя за 200 милисекунди, визуализира се картинката за натиснато състояние, след което се връща
към началното състояние.
1.2. Start - начало на нова игра. Бутонът не е активен по време на играта.
1.3. Info - показване/скриване на правилата на играта. Бутонът е постоянно активен.
1.4. Ready - маркира край на хода на текущия играч и начало на хода на следващия играч.
1.5. Undo - ако текущия играч не е маркирал край на хода си след поставяне на фигура, при на тискане на бутона, фигурата
му се изтрива и играча може да постави нова фигура.

2. Звуци:
2.1. Бекграунд звук - тематична мелодия, която звучи тихо за фон на играта.
2.2. Звук при натискате на Start
2.3. Звук при натискане на Info
2.4. Звук при поставяне на фигура от играч

3.Ако някой от играчите печели, за кратко да се появи надпис "име на играта"
4.Ако никой не спечели, да се изписва "PARITY"
