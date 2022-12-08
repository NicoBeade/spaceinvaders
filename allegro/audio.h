#ifndef AUDIO_H
#define AUDIO_H


#define INITAUDIO audios[COLISION_ALIEN_TOCADO].sample = al_load_sample("game/audioFiles/alien_tocado.wav");\
    audios[COLISION_ALIEN_TOCADO].volume = 1.0;\
    audios[COLISION_ALIEN_MUERTO].sample = al_load_sample("game/audioFiles/alien_muerto.wav");\
    audios[COLISION_ALIEN_MUERTO].volume = 0.5;\
    audios[COLISION_USER_TOCADO].sample = al_load_sample("game/audioFiles/user_tocado.wav");\
    audios[COLISION_USER_TOCADO].volume = 1.0;\
    audios[COLISION_USER_MUERTO].sample = al_load_sample("game/audioFiles/user_muerto.wav");\
    audios[COLISION_USER_MUERTO].volume = 1.0;\
    audios[BALA_USER].sample = al_load_sample("game/audioFiles/shoot.wav");\
    audios[BALA_USER].volume = 0.3;\
    audios[COLISION_MOTHERSHIP_MUERTA].sample = al_load_sample("game/audioFiles/mothership_muerta.wav");\
    audios[COLISION_MOTHERSHIP_MUERTA].volume = 1.0;\
    audios[COLISION_BARRERA_TOCADA].sample = al_load_sample("game/audioFiles/choque_balas_1.wav");\
    audios[COLISION_BARRERA_TOCADA].volume = 1.0;\
    audios[COLISION_BARRERA_MUERTA].sample = al_load_sample("game/audioFiles/barrera_muerta_1.wav");\
    audios[COLISION_BARRERA_MUERTA].volume = 1.0;\
    audios[MOTHERSHIP_APARECE].sample = al_load_sample("game/audioFiles/mothership_aparece_1.wav");\
    audios[MOTHERSHIP_APARECE].volume = 1.0;\
    audios[MOVIMIENTO_ALIENS].sample = al_load_sample("game/audioFiles/movimiento_aliens.wav");\
    audios[MOVIMIENTO_ALIENS].volume = 1.0;\
    audios[BALA_ALIEN].sample = al_load_sample("game/audioFiles/bala_alien.wav");\
    audios[BALA_ALIEN].volume = 1.0;\
    audios[SELECT_MENU].sample = al_load_sample("game/audioFiles/select_menu.wav");\
    audios[SELECT_MENU].volume = 1.0;\
    audios[SWAP_MENU].sample = al_load_sample("game/audioFiles/swap_menu.wav");\
    audios[SWAP_MENU].volume = 1.0;\
    audios[ERROR_MENU].sample = al_load_sample("game/audioFiles/error_menu.wav");\
    audios[ERROR_MENU].volume = 1.0;\
    audios[SWEEP_LETRA].sample = al_load_sample("game/audioFiles/sweep_letra.wav");\
    audios[SWEEP_LETRA].volume = 1.0;\
    audios[SAVED_SCORE].sample = al_load_sample("game/audioFiles/saved_score.wav");\
    audios[SAVED_SCORE].volume = 1.0;\
    audios[COLISION_CHOQUE_BALAS].sample = al_load_sample("game/audioFiles/choque_balas.wav");\
    audios[COLISION_CHOQUE_BALAS].volume = 1.0;\
    audios[PARTIDA_GANADA].sample = al_load_sample("game/audioFiles/partida_ganada.wav");\
    audios[PARTIDA_GANADA].volume = 1.0;\
    audios[PARTIDA_PERDIDA].sample = al_load_sample("game/audioFiles/partida_perdida.wav");\
    audios[PARTIDA_PERDIDA].volume = 1.0;\
    
#define INITMUSICA musica[MUSICA_MENU- AUDIOMAX].sample = al_load_sample("game/audioFiles/musica_menu.wav");\
    musica[MUSICA_MENU- AUDIOMAX].volume = 0.8;\
    musica[MUSICA_JUEGO - AUDIOMAX].sample = al_load_sample("game/audioFiles/musica_juego.wav");\
    musica[MUSICA_JUEGO - AUDIOMAX].volume = 1.0;

#endif