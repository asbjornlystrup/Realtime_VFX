#include "prerenderUtils.h"
#include "image.h"

void prerenderFlipbookMaps(std::string directory, int keyframeCount) {
	for (int keyframeIndex = 0; keyframeIndex < keyframeCount; keyframeIndex++) {
		Image *RTFImage = new Image(directory + "rightTopFront" + std::to_string(keyframeIndex) + ".png");
		Image *LBoBaImage = new Image(directory + "leftBottomBack" + std::to_string(keyframeIndex) + ".png");

		glm::ivec2 size = RTFImage->getSize();

		unsigned char *RTFPixelData = RTFImage->getPixelData();
		unsigned char *LBoBaPixelData = LBoBaImage->getPixelData();
		unsigned char *normalPixelData = new unsigned char[size.x * size.y * 3];
		unsigned char *diffuseAlphaPixelData = new unsigned char[size.x * size.y * 4];

		for (int y = 0; y < size.y; y++) {
			for (int x = 0; x < size.x; x++) {
				glm::vec4 RTFColor;
				int base = (y * size.x + x) * 4;
				RTFColor.r = (float)(RTFPixelData[base]) / 255.0f;
				RTFColor.g = (float)(RTFPixelData[base + 1]) / 255.0f;
				RTFColor.b = (float)(RTFPixelData[base + 2]) / 255.0f;
				RTFColor.a = (float)(RTFPixelData[base + 3]) / 255.0f;

				glm::vec4 LBoBaColor;
				LBoBaColor.r = (float)(LBoBaPixelData[base]) / 255.0f;
				LBoBaColor.g = (float)(LBoBaPixelData[base + 1]) / 255.0f;
				LBoBaColor.b = (float)(LBoBaPixelData[base + 2]) / 255.0f;
				LBoBaColor.a = (float)(LBoBaPixelData[base + 3]) / 255.0f;

				// Start of calculation

				glm::vec4 normalColor = glm::vec4(0);
				glm::vec4 diffuseAlphaColor = glm::vec4(0);

				float lightness = (RTFColor.x + RTFColor.y + RTFColor.z + LBoBaColor.x + LBoBaColor.y + LBoBaColor.z) / 5.0f;
				diffuseAlphaColor.r = lightness;
				diffuseAlphaColor.g = lightness;
				diffuseAlphaColor.b = lightness;
				diffuseAlphaColor.a = RTFColor.a;

				//glm::vec3 normal;
				//normal.x = LBoBaColor.x - RTFColor.x;
				//normal.y = RTFColor.y - LBoBaColor.y;
				//normal.z = glm::sqrt(1.0f - normal.x * normal.x - normal.y * normal.y);


				glm::vec3 normal = glm::vec3(RTFColor.x - LBoBaColor.x, RTFColor.y - LBoBaColor.y, RTFColor.z - LBoBaColor.z);
				//glm::vec3 transparencyNormal = glm::vec3(0, 0, 1);

				/*if (glm::length(normal) > 1.0f) {
				normal = glm::normalize(normal);
				}*/

				//printf("Length: %f\n", glm::length(normal));

				normal = normal * 0.5f + 0.5f;
				//transparencyNormal = transparencyNormal * 0.5f + 0.5f;

				normalColor = glm::vec4(normal, 1.0f); //glm::vec4(glm::mix(transparencyNormal, normal, diffuseAlphaColor.a), 1.0f);

													   // End of calculation

				normalColor = glm::clamp(normalColor, 0.0f, 1.0f);
				diffuseAlphaColor = glm::clamp(diffuseAlphaColor, 0.0f, 1.0f);

				base = (y * size.x + x) * 3;
				normalPixelData[base] = (char)(normalColor.r * 255.0);
				normalPixelData[base + 1] = (char)(normalColor.g * 255.0);
				normalPixelData[base + 2] = (char)(normalColor.b * 255.0);

				base = (y * size.x + x) * 4;
				diffuseAlphaPixelData[base] = (char)(diffuseAlphaColor.r * 255.0);
				diffuseAlphaPixelData[base + 1] = (char)(diffuseAlphaColor.g * 255.0);
				diffuseAlphaPixelData[base + 2] = (char)(diffuseAlphaColor.b * 255.0);
				diffuseAlphaPixelData[base + 3] = (char)(diffuseAlphaColor.a * 255.0);
			}
		}

		Image *normalImage = new Image(size, 3, normalPixelData);
		Image *diffuseAlphaImage = new Image(size, 4, diffuseAlphaPixelData);

		normalImage->save(directory + "normalMap" + std::to_string(keyframeIndex) + ".png");
		diffuseAlphaImage->save(directory + "diffuseAlphaMap" + std::to_string(keyframeIndex) + ".png");

		delete normalPixelData;
		delete diffuseAlphaPixelData;
		delete RTFImage;
		delete LBoBaImage;
		delete normalImage;
		delete diffuseAlphaImage;
	}
}