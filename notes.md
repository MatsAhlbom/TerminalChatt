1. Vad ska klienten skicka när den ansluter?
	En fråga om att gå med, som svaras med ett login promt
2. Hur väljer användaren namn?
	Den promtar upp en login sida först
3. Hur ska ett vanligt meddelande se ut?
	[användare TT:MM]#: Meddelande
4. Hur ska en användare lämna chatten?
	/exit
5. Ska servern skicka systemmeddelanden?
	Ja, t.ex [användare lämnade], [användare anslöt]
6. Ska avsändaren själv få tillbaka sitt eget meddelande?
	Ja
7. Hur ska tomma meddelanden hanteras?
	Inte tillåt, kontolleras lokalt och svaras med en varning 
8. Hur långt får ett meddelande vara?
	Det får max vara 128 chars
9. Vad händer om anvädaren skickar mer data än vad bufferten klarar?
	Det borde hanteras lokalt och svaras med en varning
