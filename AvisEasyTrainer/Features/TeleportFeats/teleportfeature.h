#pragma once
namespace feature {
	namespace teleoptions {


        inline bool tickTeleportNow = false;
        inline RED4ext::Vector3 pendingTeleportPosition{};
        inline RED4ext::Vector3 pendingTeleportRotation{};

        void RequestTeleport(const RED4ext::Vector3& pos);
        void TeleportForward(float distance = 2.0f);
        void TeleportUp(float amount = 2.0f);
        void TeleportDown(float amount = 2.0f);
        void Tick();

		struct TeleportLocation
		{
			std::string name;       
			std::string category;     
            RED4ext::Vector3 position;
		};


        // All teleport locations used in this file were originally sourced from a public CET lue script Simplemenu.
	    inline std::vector<TeleportLocation> teleportLocations = {
            { "Akulov Penthouse", "Location", { -1218.135986f, 1409.635010f, 113.524445f } },
            { "Arasaka Orbital Station", "Location", { 4743.650879f, -1091.755127f, 1310.439575f } },
            { "Arasaka Tower Jungle", "Location", { -1449.256470f, 118.300171f, 321.639038f } },
            { "Arasaka Tower Temple", "Location", { -1383.655518f, 118.832474f, 542.696289f } },
            { "Badlands Tunnel Entrance", "Location", { 185.345749f, 2365.449707f, 67.081177f } },
            { "Blackwall Tunnel", "Location", { -1663.618774f, -1867.443726f, 54.990150f } },
            { "Clouds (inside)", "Location", { -652.481812f, 790.145996f, 128.252228f } },
            { "Johnny Interrogation Room", "Location", { -1389.446533f, 141.266556f, -139.361572f } },
            { "Konpeki Plaza Hotel Suite", "Location", { -2202.186035f, 1783.184204f, 163.000000f } },
            { "Megabuilding H10 Top", "Location", { -1371.780029f, 1340.888550f, 311.471313f } },
            { "Megabuilding H8 Top", "Location", { -701.484680f, 849.270264f, 322.252228f } },
            { "NCPD Room", "Location", { -1761.547729f, -1010.821655f, 94.300003f } },
            { "Afterlife", "Location", { -1456.893433f, 1038.277222f, 16.825035f } },
            { "Clouds", "Location", { -665.472961f, 810.591492f, 128.273163f } },
            { "Delamain's HQ", "Location", { -940.837341f, -77.526871f, 7.509773f } },
            { "El Coyote Cojo", "Location", { -1260.774536f, -981.771790f, 11.589195f } },
            { "Grand Imperial Mall", "Location", { -2265.841309f, -2112.402588f, 13.296661f } },
            { "Jig Jig Street", "Location", { -640.769165f, 886.267151f, 19.888809f } },
            { "Lizzie's Bar", "Location", { -1207.988525f, 1563.142090f, 22.920128f } },
            { "Mobile Camp", "Location", { 3444.628174f, -365.633270f, 133.852707f } },
            { "Nomad Camp", "Location", { 1816.382324f, 2256.925781f, 180.260223f } },
            { "No-Tell Motel", "Location", { -1158.889404f, 1342.452271f, 19.943626f } },
            { "Pacifica District", "Location", { -1751.548462f, -1933.493042f, 61.524582f } },
            { "Reconciliation Park", "Location", { -1782.537598f, -390.172638f, -4.015121f } },
            { "Rocky Ridge", "Location", { 2599.074707f, -33.218079f, 80.714417f } },
            { "Sunset Motel", "Location", { 1628.261475f, -775.431030f, 49.980309f } },
            { "Claire's Garage", "NPC", { -645.418945f, -1260.975586f, 9.376778f } },
            { "Dakota Smith", "NPC", { 2419.131836f, -795.221985f, 66.996750f } },
            { "Dino Dinovic", "NPC", { -1967.008423f, 369.847382f, 8.040825f } },
            { "Judy's House", "NPC", { -906.306396f, 1868.635620f, 42.360016f } },
            { "Misty's Esoterica", "NPC", { -1546.295776f, 1194.164063f, 16.260002f } },
            { "Mumar 'El Caliptan' Reyes", "NPC", { 405.594482f, -2352.642578f, 182.027740f } },
            { "Regina Jones", "NPC", { -1149.433105f, 1581.234619f, 71.712402f } },
            { "River's House", "NPC", { 1235.896606f, -504.580139f, 36.427094f } },
            { "Rogue Amendiares", "NPC", { -1427.401245f, 1014.764099f, 16.901749f } },
            { "Sebastian 'Padre' Ibarra", "NPC", { -1803.299805f, -1279.714111f, 21.837990f } },
            { "Wakako Okada", "NPC", { -668.265747f, 823.310669f, 19.566063f } },
            { "Watson - Northside", "Perk Shard", { -964.2407f, 2778.072f, 30.049217f } },
            { "Westbrook - Japantown", "Perk Shard", { -399.95956f, 254.22954f, 22.14943f } },
            { "Santo Domingo - Arroyo (1)", "Perk Shard", { -874.67896f, -1008.6402f, 11.369972f } },
            { "Santo Domingo - Arroyo (2)", "Perk Shard", { -253.68695f, -1462.9231f, 7.5999146f } },
            { "Santo Domingo - Arroyo (3)", "Perk Shard", { -254.02971f, -1508.8351f, 12.610001f } },
            { "Heywood - The Glen", "Perk Shard", { -1984.3334f, -1027.2446f, 7.6319275f } },
            { "City Center - Corpo Plaza", "Perk Shard", { -1354.8429f, 444.22794f, 13.151001f } },
            { "Santo Domingo - Rancho Coronado", "Perk Shard", { 645.40015f, -2159.7837f, 39.349243f } },
            { "Badlands", "Perk Shard", { 2287.9966f, -1051.378f, 55.498795f } },
            { "Appel De Paris", "Clothing Vendor", { -1575.944580f, -282.437988f, -4.425003f } },
            { "Blossoming Sakura Clothier", "Clothing Vendor", { -682.614563f, 1239.223755f, 37.966957f } },
            { "Jinguji", "Clothing Vendor", { -1884.361938f, 82.698013f, 7.519997f } },
            { "Kabuki Store", "Clothing Vendor", { -1119.808105f, 1752.149658f, 33.722076f } },
            { "Lele Park Store", "Clothing Vendor", { -230.233231f, -36.969742f, 0.883064f } },
            { "Little China North Store", "Clothing Vendor", { -1522.441040f, 1701.879639f, 18.317543f } },
            { "Mallagra&Manzanita Store", "Clothing Vendor", { 253.300400f, -1475.245850f, 9.500000f } },
            { "Manufactory Market Vendor", "Clothing Vendor", { -1017.764709f, -1557.709351f, 25.700897f } },
            { "Pershing ST Store", "Clothing Vendor", { -1895.527832f, 2504.133301f, 18.263504f } },
            { "Red Peaks Vendor", "Clothing Vendor", { 1202.813965f, -570.510498f, 32.692131f } },
            { "Wellsprings Store", "Clothing Vendor", { -2437.346680f, -666.163452f, 6.922104f } },
            { "West Wind Estate Store", "Clothing Vendor", { -2477.911377f, -2536.459229f, 16.969376f } },
            { "Coach Fred", "Melee Vendor", { -1430.860107f, 1335.109497f, 119.206131f } },
            { "Crescent&Broad Vendor", "Melee Vendor", { -337.444519f, 563.404053f, 38.349251f } },
            { "Rancho Coronado Vendor", "Melee Vendor", { -476.260040f, -1942.191772f, 7.003807f } },
            { "Tango Tors Vendor", "Melee Vendor", { 133.805145f, -4670.488281f, 54.607399f } },
            { "West Wind Vendor", "Melee Vendor", { -2529.142578f, -2468.510010f, 17.196762f } },
            { "Chang Hoon Nam", "Netrunner Vendor", { -492.061035f, 583.292725f, 26.802223f } },
            { "Costa View Netrunner", "Netrunner Vendor", { -1906.515015f, -1925.094238f, 48.903023f } },
            { "Japantown Netrunner", "Netrunner Vendor", { -351.593842f, 1368.778564f, 42.124115f } },
            { "Yoko Tsuru", "Netrunner Vendor", { -1180.311279f, 2041.457520f, 20.087074f } },
            { "Buck's Clinic", "Ripperdoc Vendor", { -1090.759155f, 2147.218262f, 13.330742f } },
            { "Cassius Ryder's Clinic", "Ripperdoc Vendor", { -1686.586182f, 2386.400879f, 18.344055f } },
            { "Cherry Blossom Market Ripperdoc", "Ripperdoc Vendor", { -712.370605f, 871.832458f, 11.982414f } },
            { "Dr. Chrome", "Ripperdoc Vendor", { -1245.325439f, 1945.930908f, 8.030479f } },
            { "Fingers M.D.", "Ripperdoc Vendor", { -573.507813f, 795.048279f, 24.906097f } },
            { "Instant Implants", "Ripperdoc Vendor", { -1040.245972f, 1440.913696f, 0.500221f } },
            { "Kraviz's Clinic", "Ripperdoc Vendor", { -40.347633f, -52.439484f, 7.179688f } },
            { "Mobile Camp Ripperdoc", "Ripperdoc Vendor", { 3438.949463f, -380.475800f, 133.569855f } },
            { "Nomad Camp Ripperdoc", "Ripperdoc Vendor", { 1814.132202f, 2274.446289f, 182.176987f } },
            { "Octavio's Clinic", "Ripperdoc Vendor", { 588.132568f, -2179.594482f, 42.437347f } },
            { "Palms View Plaza Ripperdoc", "Ripperdoc Vendor", { -2361.011475f, -929.024597f, 12.266129f } },
            { "Skyline & Republic Ripperdoc", "Ripperdoc Vendor", { -2411.207764f, 393.523010f, 11.837067f } },
            { "Viktor's Clinic", "Ripperdoc Vendor", { -1546.726196f, 1227.393066f, 11.520233f } },
            { "Vista del Rey Ripperdoc", "Ripperdoc Vendor", { -705.582397f, -395.248322f, 8.199997f } },
            { "West Wind Ripperdoc", "Ripperdoc Vendor", { -2607.956787f, -2498.076660f, 17.334549f } },
            { "Wolleson ST Ripperdoc", "Ripperdoc Vendor", { -1072.172729f, -1274.062866f, 11.456871f } },
            { "Cannery Plaza Vendor", "Weapons Vendor", { -2402.214355f, -630.521790f, 6.906044f } },
            { "College ST Vendor", "Weapons Vendor", { -906.048767f, -703.476807f, 8.237724f } },
            { "Fuel Station Vendor", "Weapons Vendor", { -1846.321899f, -4295.308105f, 74.014191f } },
            { "Halsey&MLK Vendor", "Weapons Vendor", { -1770.706543f, 222.652618f, 43.727768f } },
            { "Iron & Lead", "Weapons Vendor", { -1895.143433f, 2729.943359f, 7.449997f } },
            { "Maelstrom Heat", "Weapons Vendor", { -783.261963f, 2183.184570f, 52.801941f } },
            { "Manufactory Market Vendor", "Weapons Vendor", { -992.885986f, -1589.419189f, 25.700897f } },
            { "Mobile Camp Vendor", "Weapons Vendor", { 3429.046387f, -375.550720f, 133.535477f } },
            { "Nomad Camp Vendor", "Weapons Vendor", { 1796.789307f, 2253.482178f, 180.262894f } },
            { "Pacifica Vendor", "Weapons Vendor", { -2438.164063f, -2405.232422f, 16.722504f } },
            { "Rancho Coronado East Vendor", "Weapons Vendor", { 569.700745f, -2201.206787f, 35.345894f } },
            { "Skyline&Salinas Vendor", "Weapons Vendor", { -453.489319f, 1450.199219f, 37.388107f } },
            { "Straight Shooter", "Weapons Vendor", { -1207.415771f, 2043.946289f, 7.844711f } },
            { "Sunset Motel Vendor", "Weapons Vendor", { 1678.147217f, -771.591980f, 49.839981f } },
            { "Ventura&Skyline Vendor", "Weapons Vendor", { -1899.170654f, -1019.690430f, 7.676468f } },
            { "Wilson Shop", "Weapons Vendor", { -1450.176147f, 1311.742676f, 119.082397f } },

        };



	}
}