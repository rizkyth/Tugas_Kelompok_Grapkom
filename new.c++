#include <GL/glut.h>
#include <cmath>

bool flip_kereta = false;
// Parameter gerakan dan animasi
float sudut_roda = 0.0f;                     // Rotasi roda kereta (derajat)
float kecepatan = 0.08f;                     // Multiplier kecepatan animasi
// Kontrol kereta oleh user
float translasi_kereta_x = 0.0f;             // Geser kereta sumbu X (A/D)
float skala_kereta = 0.6f;                   // Ukuran kereta (W/S), range [0.1, 3.0]
float pohon1_x = 2.5f;
float pohon1_z = 10.0f;
float pohon2_x = 2.0f;
float pohon2_z = -4.0f;
// Rizky
void gambar_kubus_skala(float skala_x, float skala_y, float skala_z) {
    glPushMatrix();
    glScalef(skala_x, skala_y, skala_z);
    glutSolidCube(1.0f);
    glPopMatrix();
}
// Rizky
void gambar_silinder(float jari_jari, float tinggi) {
    GLUquadric* quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    glPushMatrix();
        // Rotasi agar silinder sejajar sumbu Y
        glRotatef(-90, 1, 0, 0);
        gluCylinder(quadric, jari_jari, jari_jari, tinggi, 24, 4);
        // Tutup bawah
        gluDisk(quadric, 0, jari_jari, 24, 1);       
        // Tutup atas
        glTranslatef(0, 0, tinggi);
        gluDisk(quadric, 0, jari_jari, 24, 1);
    glPopMatrix();
    
    gluDeleteQuadric(quadric);
}
// Rizky
void gambar_roda(float jari_jari, float tebal) {
    // Velg: Torus berwarna abu-abu gelap
    glPushMatrix();
        glColor3f(0.06f, 0.06f, 0.06f);
        glutSolidTorus(jari_jari * 0.12f, jari_jari * 0.9f, 24, 48);
    glPopMatrix();
    // Hub: Bola di tengah roda
    glPushMatrix();
        glColor3f(0.2f, 0.2f, 0.2f);
        glutSolidSphere(jari_jari * 0.25f, 12, 12);
    glPopMatrix();
}
// Wildan
void gambar_pohon() {
    // ---- BATANG POHON ----
    // Material: cokelat tua
    glPushMatrix();
        glColor3f(0.45f, 0.27f, 0.07f);
        glTranslatef(0.0f, 1.0f, 0.0f);
        glRotatef(-90, 1, 0, 0);
        GLUquadric* quadric = gluNewQuadric();
        gluQuadricNormals(quadric, GLU_SMOOTH);
        gluCylinder(quadric, 0.15, 0.12, 2.0, 12, 3);
        gluDeleteQuadric(quadric);
    glPopMatrix();
    // ---- DAUN POHON ----
    // Tiga bola hijau dalam formasi piramida
    glColor3f(0.0f, 0.6f, 0.1f);
    // Daun atas (tengah)
    glPushMatrix();
        glTranslatef(0.0f, 2.3f, 0.0f);
        glutSolidSphere(0.7, 20, 20);
    glPopMatrix();
    // Daun bawah kiri
    glPushMatrix();
        glTranslatef(-0.45f, 1.9f, 0.0f);
        glutSolidSphere(0.45, 16, 16);
    glPopMatrix();   
    // Daun bawah kanan
    glPushMatrix();
        glTranslatef(0.45f, 1.9f, 0.0f);
        glutSolidSphere(0.45, 16, 16);
    glPopMatrix();
}
// Rizky
void gambar_rel(float offset) {
    // ---- REL KIRI & KANAN ----
    glColor3f(0.35f, 0.35f, 0.35f);
    
    // Rel kiri (Z = +1.2)
    glPushMatrix();
        glTranslatef(offset, -0.9f, 1.2f);
        gambar_kubus_skala(50.0f, 0.15f, 0.15f);
    glPopMatrix();
    
    // Rel kanan (Z = -1.2)
    glPushMatrix();
        glTranslatef(offset, -0.9f, -1.2f);
        gambar_kubus_skala(50.0f, 0.15f, 0.15f);
    glPopMatrix();

    // ---- BANTALAN (SLEEPERS) ----
    glColor3f(0.45f, 0.28f, 0.12f);
    for (float i = -50.0f; i <= 50.0f; i += 1.5f) {
        glPushMatrix();
            glTranslatef(i + offset, -0.95f, 0.0f);
            gambar_kubus_skala(0.9f, 0.08f, 2.6f);
        glPopMatrix();
    }
}
// Rizky
void gambar_lokomotif(float offset_x) {
    // ---- BODY UTAMA ----
    // Warna: merah cerah
    glColor3f(0.8f, 0.12f, 0.12f);
    glPushMatrix();
        glTranslatef(-2.0f + offset_x, 0.0f, 0.0f);
        gambar_kubus_skala(4.0f, 1.2f, 2.2f);
    glPopMatrix();
    // ---- KABIN MASINIS ----
    // Warna: oranye muda
    glColor3f(0.9f, 0.4f, 0.3f);
    glPushMatrix();
        glTranslatef(-0.5f + offset_x, 1.0f, 0.0f);
        gambar_kubus_skala(1.2f, 1.4f, 1.8f);
    glPopMatrix();
    // ---- CEROBONG ----
    // Warna: abu-abu gelap
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix();
        glTranslatef(-3.5f + offset_x, 1.0f, 0.0f);
        glRotatef(-90, 1, 0, 0);
        GLUquadric* quadric = gluNewQuadric();
        gluQuadricNormals(quadric, GLU_SMOOTH);
        gluCylinder(quadric, 0.35, 0.25, 0.8, 12, 3);
        gluDeleteQuadric(quadric);
    glPopMatrix();
    // ---- JENDELA ----
    // Warna: biru untuk kaca
    glColor3f(0.1f, 0.4f, 0.9f);
    // Jendela depan kiri
    glPushMatrix();
        glTranslatef(-0.6f + offset_x, 1.0f, 1.05f);
        gambar_kubus_skala(0.6f, 0.6f, 0.05f);
    glPopMatrix();   
    // Jendela depan kanan
    glPushMatrix();
        glTranslatef(-0.6f + offset_x, 1.0f, -1.05f);
        gambar_kubus_skala(0.6f, 0.6f, 0.05f);
    glPopMatrix();
}
// Rizky
void gambar_gerbong(float offset_x) {
    // Warna: biru cargo
    glColor3f(0.2f, 0.5f, 0.8f);
    glPushMatrix();
        glTranslatef(3.0f + offset_x, 0.0f, 0.0f);
        gambar_kubus_skala(4.0f, 1.3f, 2.0f);
    glPopMatrix();
}
// Rizky
void gambar_kereta_lengkap() {
    // Offset posisi: gerbong di depan, lokomotif di belakang
    float offset_lokomotif = 5.0f;
    float offset_gerbong = -5.0f;
    // Gambar gerbong dan lokomotif
    gambar_gerbong(offset_gerbong);
    gambar_lokomotif(offset_lokomotif);
    // ---- RODA LOKOMOTIF ----
    glColor3f(0.06f, 0.06f, 0.06f);
    // Roda besar kiri lokomotif
    glPushMatrix();
        glTranslatef(-3.0f + offset_lokomotif, -0.9f, 1.0f);
        glRotatef(sudut_roda, 0, 0, 1);  // Rotasi sepanjang sumbu Z
        gambar_roda(0.5f, 0.2f);
    glPopMatrix();
    // Roda besar kanan lokomotif
    glPushMatrix();
        glTranslatef(-3.0f + offset_lokomotif, -0.9f, -1.0f);
        glRotatef(sudut_roda, 0, 0, 1);
        gambar_roda(0.5f, 0.2f);
    glPopMatrix();
    // Roda kecil kiri lokomotif
    glPushMatrix();
        glTranslatef(-1.0f + offset_lokomotif, -0.85f, 1.0f);
        glRotatef(sudut_roda, 0, 0, 1);
        gambar_roda(0.35f, 0.12f);
    glPopMatrix();
    // Roda kecil kanan lokomotif
    glPushMatrix();
        glTranslatef(-1.0f + offset_lokomotif, -0.85f, -1.0f);
        glRotatef(sudut_roda, 0, 0, 1);
        gambar_roda(0.35f, 0.12f);
    glPopMatrix();
    // ---- RODA GERBONG ----
    // Roda gerbong kiri
    glPushMatrix();
        glTranslatef(3.0f + offset_gerbong, -0.85f, 1.0f);
        glRotatef(sudut_roda, 0, 0, 1);
        gambar_roda(0.38f, 0.12f);
    glPopMatrix();   
    // Roda gerbong kanan
    glPushMatrix();
        glTranslatef(3.0f + offset_gerbong, -0.85f, -1.0f);
        glRotatef(sudut_roda, 0, 0, 1);
        gambar_roda(0.38f, 0.12f);
    glPopMatrix();
}
// rizky
void gambar_tanah() {
    // Simpan status GL_COLOR_MATERIAL
    GLboolean warna_material_aktif = glIsEnabled(GL_COLOR_MATERIAL);
    if (warna_material_aktif) glDisable(GL_COLOR_MATERIAL);
    // Material rumput hijau
    GLfloat ambient_rumput[] = {0.05f, 0.18f, 0.05f, 1.0f};
    GLfloat diffuse_rumput[] = {0.18f, 0.6f, 0.18f, 1.0f};
    GLfloat specular_rumput[] = {0.02f, 0.02f, 0.02f, 1.0f};
    GLfloat kilauan_rumput = 2.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_rumput);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_rumput);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular_rumput);
    glMaterialf(GL_FRONT, GL_SHININESS, kilauan_rumput);
    // Gambar kubus besar sebagai ground
    glPushMatrix();
        glTranslatef(0.0f, -1.6f, 0.0f);
        gambar_kubus_skala(200.0f, 0.2f, 200.0f);
    glPopMatrix();
    // Kembalikan status GL_COLOR_MATERIAL
    if (warna_material_aktif) glEnable(GL_COLOR_MATERIAL);
}
// yusuf
void gambar_gunung_krater() {
    float size = 40.0f;
    float step = 1.5f;
    float offsetZ = -120.0f;
    for (float x = -size; x < size; x += step) {
        for (float z = -size; z < size; z += step) {
            float d1 = sqrt(x*x + z*z);
            float d2 = sqrt((x+step)*(x+step) + z*z);
            float d3 = sqrt((x+step)*(x+step) + (z+step)*(z+step));
            float d4 = sqrt(x*x + (z+step)*(z+step));

            float h1 = 18.0f - fabs(d1 - 12.0f);
            float h2 = 18.0f - fabs(d2 - 12.0f);
            float h3 = 18.0f - fabs(d3 - 12.0f);
            float h4 = 18.0f - fabs(d4 - 12.0f);

            if (h1 < 0) h1 = 0;
            if (h2 < 0) h2 = 0;
            if (h3 < 0) h3 = 0;
            if (h4 < 0) h4 = 0;

            if (h1 > 14)
                glColor3f(0.6f, 0.6f, 0.6f);
            else if (h1 > 7)
                glColor3f(0.2f, 0.7f, 0.2f);
            else
                glColor3f(0.5f, 0.4f, 0.3f);

            glBegin(GL_QUADS);
                glVertex3f(x,        h1, z + offsetZ);
                glVertex3f(x+step,  h2, z + offsetZ);
                glVertex3f(x+step,  h3, z + step + offsetZ);
                glVertex3f(x,        h4, z + step + offsetZ);
            glEnd();
        }
    }
}

// nauval
void gambar_kotak(float lebar, float tinggi, float kedalaman) {
    float x = lebar / 2, y = tinggi / 2, z = kedalaman / 2;
    glBegin(GL_QUADS);
    // Depan
    glVertex3f(-x, -y, z); glVertex3f(x, -y, z); 
    glVertex3f(x, y, z); glVertex3f(-x, y, z);
    // Belakang
    glVertex3f(x, -y, -z); glVertex3f(-x, -y, -z); 
    glVertex3f(-x, y, -z); glVertex3f(x, y, -z);
    // Kiri
    glVertex3f(-x, -y, -z); glVertex3f(-x, -y, z); 
    glVertex3f(-x, y, z); glVertex3f(-x, y, -z);
    // Kanan
    glVertex3f(x, -y, z); glVertex3f(x, -y, -z); 
    glVertex3f(x, y, -z); glVertex3f(x, y, z);
    // Atas
    glVertex3f(-x, y, z); glVertex3f(x, y, z); 
    glVertex3f(x, y, -z); glVertex3f(-x, y, -z);
    // Bawah
    glVertex3f(-x, -y, -z); glVertex3f(x, -y, -z); 
    glVertex3f(x, -y, z); glVertex3f(-x, -y, z);
    glEnd();
}
// nauval
void gambar_atap(float lebar_atas, float lebar_bawah, float tinggi, float kedalaman) {
    float z = kedalaman / 2;
    float atas = lebar_atas / 2;
    float bawah = lebar_bawah / 2;
    float y_atas = tinggi / 2;
    float y_bawah = -tinggi / 2;
    glBegin(GL_QUADS);
    glVertex3f(-bawah, y_bawah, z); glVertex3f(bawah, y_bawah, z); 
    glVertex3f(atas, y_atas, z); glVertex3f(-atas, y_atas, z);
    glVertex3f(bawah, y_bawah, -z); glVertex3f(-bawah, y_bawah, -z); 
    glVertex3f(-atas, y_atas, -z); glVertex3f(atas, y_atas, -z);
    glVertex3f(-bawah, y_bawah, -z); glVertex3f(bawah, y_bawah, -z); 
    glVertex3f(bawah, y_bawah, z); glVertex3f(-bawah, y_bawah, z);
    glVertex3f(-atas, y_atas, z); glVertex3f(atas, y_atas, z); 
    glVertex3f(atas, y_atas, -z); glVertex3f(-atas, y_atas, -z);
    glVertex3f(-bawah, y_bawah, z); glVertex3f(-bawah, y_bawah, -z); 
    glVertex3f(-atas, y_atas, -z); glVertex3f(-atas, y_atas, z);
    glVertex3f(atas, y_atas, z); glVertex3f(atas, y_atas, -z); 
    glVertex3f(bawah, y_bawah, -z); glVertex3f(bawah, y_bawah, z);
    glEnd();
}
// nauval
void gambar_lingkaran_3d(float jari_jari, float kedalaman) {
    int segmen = 40;
    float z = kedalaman / 2;
    // Depan (TRIANGLE_FAN)
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, z);
    for (int i = 0; i <= segmen; i++) {
        float sudut = 2.0f * 3.1415926f * i / segmen;
        glVertex3f(cos(sudut) * jari_jari, sin(sudut) * jari_jari, z);
    }
    glEnd();
    // Belakang (TRIANGLE_FAN)
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, -z);
    for (int i = 0; i <= segmen; i++) {
        float sudut = 2.0f * 3.1415926f * i / segmen;
        glVertex3f(cos(sudut) * jari_jari, sin(sudut) * jari_jari, -z);
    }
    glEnd();
    // Sisi (QUAD_STRIP)
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segmen; i++) {
        float sudut = 2.0f * 3.1415926f * i / segmen;
        float x = cos(sudut) * jari_jari;
        float y = sin(sudut) * jari_jari;
        glVertex3f(x, y, z);
        glVertex3f(x, y, -z);
    }
    glEnd();
}
// nauval
void gambar_jendela_bulat() {
    GLfloat warna_bingkai[] = {0.2f, 0.1f, 0.05f, 1.0f};
    GLfloat warna_kaca[] = {0.5f, 0.8f, 1.0f, 0.5f};
    // Bingkai jendela
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, warna_bingkai);
    gambar_lingkaran_3d(0.35, 0.08);
    // Kaca dengan transparansi
    glPushMatrix();
    glTranslatef(0, 0, 0.05);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, warna_kaca);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    gambar_lingkaran_3d(0.25, 0.02);
    glDisable(GL_BLEND);
    glPopMatrix();
}
// nauval
void gambar_rumah_di_posisi(float pos_x, float pos_z) {
    glPushMatrix();
    glTranslatef(pos_x, 0.6f, pos_z);
    // ---- DINDING UTAMA ----
    GLfloat warna_dinding[] = {0.86f, 0.76f, 0.65f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, warna_dinding);
    gambar_kotak(3.0f, 2.0f, 2.2f);
    // ---- PINTU ----
    GLfloat warna_pintu[] = {0.5f, 0.3f, 0.1f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, warna_pintu);
    glPushMatrix();
    glTranslatef(1.3f - 1.5f, -0.5f, 1.11f);
    gambar_kotak(0.6f, 1.2f, 0.05f);
    glPopMatrix();
    // ---- JENDELA DEPAN (KIRI DAN KANAN) ----
    glPushMatrix();
    glTranslatef(-0.6f, 0.2f, 1.12f);
    gambar_jendela_bulat();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.6f, 0.2f, 1.12f);
    gambar_jendela_bulat();
    glPopMatrix();
    // ---- JENDELA SAMPING ----
    glPushMatrix();
    glTranslatef(1.12f, 0.2f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gambar_jendela_bulat();
    glPopMatrix();
    // ---- ATAP ----
    GLfloat warna_atap[] = {0.6f, 0.1f, 0.1f, 1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, warna_atap);
    glTranslatef(0.0f, 1.2f, 0.0f);
    gambar_atap(1.6f, 3.2f, 0.7f, 2.3f);
    glPopMatrix();
}
// yusuf
void drawSphere(float x, float y, float z, float r) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidSphere(r, 32, 32);
    glPopMatrix();
}
//yusuf
void drawCloud(float scale) {
    glPushMatrix();
    glScalef(scale, scale, scale);
    glColor3f(0.97f, 0.97f, 0.97f);
    drawSphere(-1.2f, 0.2f, 0.0f, 0.9f);
    drawSphere(-0.3f, 0.5f, 0.2f, 1.1f);
    drawSphere(0.7f, 0.4f, -0.2f, 1.0f);
    drawSphere(1.6f, 0.2f, 0.0f, 0.85f);
    drawSphere(-0.6f, -0.1f, 0.1f, 0.8f);
    drawSphere(0.6f, -0.1f, -0.1f, 0.8f);
    glPopMatrix();
}
// YUSUF
void drawSun() {
    glPushMatrix();
    glTranslatef(0.0f, 5.2f, -2.0f);
    glScalef(0.4f, 0.4f, 0.4f);
    glColor3f(1.0f, 0.95f, 0.0f);
    glutSolidSphere(1.4, 40, 40);
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidTorus(0.08, 1.55, 30, 60);
    // subtle decorative rays (disable lighting briefly)
    glDisable(GL_LIGHTING);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.9f, 0.0f);
    for (int i = 0; i < 360; i += 18) {
        float a1 = i * 3.14159f / 180.0f;
        float a2 = (i + 9) * 3.14159f / 180.0f;
        glVertex3f(cos(a1)*1.6f, sin(a1)*1.6f, 0.0f);
        glVertex3f(cos(a2)*1.6f, sin(a2)*1.6f, 0.0f);
        glVertex3f(cos((a1+a2)/2)*2.5f, sin((a1+a2)/2)*2.5f, 0.0f);
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();
}
// Rizky
void tampilkan_scene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // ---- SETUP KAMERA ----
    // Kamera frontal terpusat di (0, 3, 16) melihat ke (0, 0, 0)
    float pos_kamera_x = 0.0f;
    float pos_kamera_y = 3.0f;
    float pos_kamera_z = 16.0f;
    gluLookAt(pos_kamera_x, pos_kamera_y, pos_kamera_z,  // Posisi mata
              0.0f, 0.0f, 0.0f,                            // Target lihat
              0.0f, 1.0f, 0.0f);                           // Vektor atas
    // ---- SETUP CAHAYA ----  //YUSUf
    GLfloat posisi_cahaya[] = {10.0f, 30.0f, 10.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, posisi_cahaya);
    // Sun and clouds (from Yusuf)
    drawSun(); 
    // Clouds positioned in the sky
    glPushMatrix(); 
    glTranslatef(-3.5f, 3.5f, -4.0f); 
    drawCloud(0.8f); 
    glPopMatrix();
    glPushMatrix(); 
    glTranslatef(3.5f, 3.8f, -7.0f); 
    drawCloud(0.6f); 
    glPopMatrix();
    glPushMatrix(); 
    glTranslatef(6.5f, 4.5f, -10.0f); 
    drawCloud(0.4f); 
    glPopMatrix();
    // ---- RENDER GROUND ---- // yusuf
    gambar_gunung_krater();
    gambar_tanah();
    // ---- RENDER REL ----
    gambar_rel(0.0f);  // Rel tetap statis di offset 0
    // ---- RENDER POHON (FIKS) ----
    // wildan
    // Pohon 1
    glPushMatrix();
        glTranslatef(pohon1_x, 0.0f, pohon1_z);
        gambar_pohon();
    glPopMatrix();
    // Pohon 2
    glPushMatrix();
        glTranslatef(pohon2_x, 0.0f, pohon2_z);
        gambar_pohon();
    glPopMatrix();
    // nauval
    // ---- RENDER RUMAH STATIS ----
    GLboolean warna_material_aktif = glIsEnabled(GL_COLOR_MATERIAL);
    if (warna_material_aktif) glDisable(GL_COLOR_MATERIAL);
    // Rumah 1: Kiri depan
    GLfloat specular1[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular1);
    glMaterialf(GL_FRONT, GL_SHININESS, 8.0f);
    gambar_rumah_di_posisi(-5.0f, 5.0f);
    // Rumah 2: Kanan belakang
    GLfloat specular2[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular2);
    glMaterialf(GL_FRONT, GL_SHININESS, 8.0f);
    gambar_rumah_di_posisi(5.0f, -5.0f);
    if (warna_material_aktif) glEnable(GL_COLOR_MATERIAL);
    // ---- RENDER KERETA DENGAN KONTROL USER ----
    // Rizky

    glPushMatrix(); // Gambar kereta
        glTranslatef(translasi_kereta_x, 0.0f, 0.0f); // Translasi kereta
        if (flip_kereta) {
            // putar 180 derajat di sumbu Y sehingga kereta menghadap arah berlawanan pada sumbu X
            glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        }
        glScalef(skala_kereta, skala_kereta, skala_kereta); // Skala kereta
        gambar_kereta_lengkap(); // Gambar kereta
    glPopMatrix(); // Akhir gambar kereta

    glutSwapBuffers();
}
// Rizky
void perbarui_animasi(int v) {
    // ---- UPDATE ROTASI RODA ----
    sudut_roda += (360.0f * kecepatan * 0.6f);
    if (sudut_roda > 360.0f) sudut_roda -= 360.0f;
    glutPostRedisplay();
    glutTimerFunc(16, perbarui_animasi, 0);
}
// Rizky
void tangani_input_keyboard(unsigned char tombol, int x, int y) {
    switch (tombol) {
        case 'a': 
        if (translasi_kereta_x > -5.0f) translasi_kereta_x -= 0.5f;
        break;
        case 'd': 
        if (translasi_kereta_x < 5.0f) translasi_kereta_x += 0.5f;
        break;
        case 'w': 
            skala_kereta += 0.05f; 
            if (skala_kereta > 3.0f) skala_kereta = 3.0f; 
            break;
        case 's': 
            skala_kereta -= 0.05f; 
            if (skala_kereta < 0.1f) skala_kereta = 0.1f; 
            break;
        case '+': kecepatan += 0.02f; break;
        case '-': 
            kecepatan -= 0.02f; 
            if (kecepatan < 0.01f) kecepatan = 0.01f; 
            break;
        case 'r':
        case 'R':
            flip_kereta = !flip_kereta;
            break;       
        case 27:  // ESC
            exit(0); 
            break;
    }
}
// Rizky 
void inisialisasi_opengl() {
    // Enable depth testing untuk hidden surface removal
    glEnable(GL_DEPTH_TEST);
    // Enable lighting dan light 0
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    // ---- SETUP CAHAYA ----
    GLfloat ambient[] = {0.05f, 0.05f, 0.05f, 1.0f};
    GLfloat diffuse[] = {0.75f, 0.75f, 0.75f, 1.0f};
    GLfloat specular[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    // ---- SETUP MATERIAL ----
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    // ---- WARNA BACKGROUND ----
    glClearColor(0.52f, 0.80f, 0.92f, 1.0f);  // Biru langit
    // ---- SETUP PROYEKSI ----
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.1, 500.0);  // FOV 60°, aspek 1:1
    glMatrixMode(GL_MODELVIEW);
}
// Rizky
int main(int argc, char** argv) {
    // Inisialisasi GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // Buat window square 800x800 (aspek 1:1)
    glutInitWindowSize(800, 800);
    glutCreateWindow("Kereta Api 3D - Dengan Variabel Indonesia");
    // Setup OpenGL
    inisialisasi_opengl();
    // Register callback functions
    glutDisplayFunc(tampilkan_scene);
    glutKeyboardFunc(tangani_input_keyboard);
    glutTimerFunc(16, perbarui_animasi, 0);
    // Mulai event loop
    glutMainLoop();
    
    return 0;
}