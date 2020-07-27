#include <global.h>
#include <checkArgs.hpp>
#include <assert.h>
#include <mutex>

uint64_t* g_numbers = nullptr;
uint64_t sumaParalelo = 0;
std:: mutex mutex;

void fillArray(size_t beginIdx, size_t endIdx, size_t min, size_t max) {

	//Used for std::uniform_int_distribution
	std::srand(std::time(0));
	std::random_device device;
        std::mt19937 rng(device());
        std::uniform_int_distribution<> unif(min, max);

        for(size_t i = beginIdx; i < endIdx; ++i){
                 g_numbers[i] = unif(rng);
		 }
        }

void sumasParalelo(size_t beginIdx,size_t endIdx){
	mutex.lock();
	for(uint32_t i = beginIdx; i < endIdx; i++){
		sumaParalelo += g_numbers[i];
	//	std::cout << sumaParalelo << " ";
	}
	mutex.unlock();

}

int main(int argc, char** argv){
	uint64_t totalElementos;
	uint32_t numThreads;
	size_t min;
	size_t max;

	auto argumentos = (std::shared_ptr<checkArgs>) new checkArgs(argc, argv);

	totalElementos = argumentos->getArgs().tamProblema;
	numThreads     = argumentos->getArgs().numThreads;
	min	       = argumentos->getArgs().minRandom;
	max            = argumentos->getArgs().maxRandom;
	
	std::cout << "  " << std::endl;
        std::cout << "Elementos: " << totalElementos << std::endl;
        std::cout << "Threads  : " << numThreads     << std::endl;
        std::cout << "Límite inferior: " << min << std::endl;
        std::cout << "Límite superior: " << max << std::endl;

	// ======LLENADO DE LA ESTRUCTURA CON NUMEROS==Secuencial
	g_numbers = new uint64_t[totalElementos];
	auto start = std::chrono::high_resolution_clock::now();

	fillArray(0, totalElementos, min, max);

	auto end     = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeFill_secuencial = elapsed.count();

	delete[] g_numbers;

	// ======LLENADO DE LA ESTRUCTURA CON NUMEROS==Threads

	g_numbers = new uint64_t[totalElementos];
	start = std::chrono::high_resolution_clock::now();
	std::vector<std::thread*> threads;

	for(size_t i = 0; i < numThreads; i++){
		threads.push_back( new std::thread(fillArray, i*(totalElementos)/numThreads, (i+1)*(totalElementos)/numThreads, min, max));

	}
	for(auto& th : threads){
		th->join();
	}
	end     = std::chrono::high_resolution_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeFill_threads = elapsed.count();

	//=====SUMA DE NUMEROS===Secuencial

	std::vector<std::thread*> threads01;
	uint64_t sumaSecuencial = 0;

	start = std::chrono::high_resolution_clock::now();

	for (size_t i=0; i< totalElementos; i++){
		sumaSecuencial+= g_numbers[i];
	}

	end     = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        auto totalTime_SumaSecuencial = elapsed.count();

	//std::cout << " " << std::endl;
        //std::cout << "Sumas parciales: ";

	//====SUMA DE NUMEROS===Threads

	start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < numThreads; i++){
		threads01.push_back(new std::thread(sumasParalelo, i*(totalElementos)/numThreads, (i+1)*(totalElementos)/numThreads));
	}

	for(auto &th01 : threads01){
		th01->join();
	}
	end     = std::chrono::high_resolution_clock::now();
        elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        auto totalTime_SumaParalelo = elapsed.count();

	//std::cout << " " << std::endl;
	//std::cout << "Numeros del arreglo: ";
 	//for (uint64_t i = 0; i < totalElementos; i++)
        //{
        //        std::cout << g_numbers[i] <<  " " ;
        //}

	std::cout << "\n";
	std::cout << "====Modulo de Llenado del arreglo====" << std::endl;
	std::cout << "Tiempo secuencial :"  << totalTimeFill_secuencial << "[ms]" << std::endl;
        std::cout << "Tiempo threads    :"  << totalTimeFill_threads<< "[ms]" << std::endl;
	std::cout << "SpeedUp           :"  << (double)totalTimeFill_secuencial/totalTimeFill_threads << "\n" << std::endl;

	std::cout << "====Modulo de Sumas del arreglo====" << std::endl;
        std::cout << "Tiempo secuencial :"  << totalTime_SumaSecuencial << "[ms]" << std::endl;
        std::cout << "Tiempo threads    :"  << totalTime_SumaParalelo << "[ms]" << std::endl;
        std::cout << "SpeedUp           :"  << (double)totalTime_SumaSecuencial/totalTime_SumaParalelo << "\n" <<std::endl;

	std::cout << "====Comprobación de sumas====" << std::endl;
	std::cout << "Suma Total serial: " << sumaSecuencial << std::endl;
        std::cout << "Suma Total en Paralelo: " << sumaParalelo << "\n" <<std::endl;
	
        std::cout << "====Tiempos totales==== " << std::endl;
        std::cout << "Tiempo total secuencial: " << totalTimeFill_secuencial + totalTime_SumaSecuencial  << std::endl;
        std::cout << "Tiempo Total paralelo: " << totalTimeFill_threads + totalTime_SumaParalelo  << "\n" <<std::endl;

	return(EXIT_SUCCESS);

}

