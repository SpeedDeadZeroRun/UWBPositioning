<?php

use Illuminate\Database\Migrations\Migration;
use Illuminate\Database\Schema\Blueprint;
use Illuminate\Support\Facades\Schema;

return new class extends Migration
{
    /**
     * Run the migrations.
     */
    public function up(): void
    {
        Schema::create('accesses', function (Blueprint $table) {
            $table->id()->primary();
            $table->string('name', 32)->unique();
            $table->string('secret');
            $table->foreignId('of_entity')->nullable()->references('id')->on('entities');
            $table->timestamps();
        });

        // Schema::create('sessions', function (Blueprint $table) {
        //     $table->string('id')->primary();
        //     $table->foreignId('access_id')->nullable()->index();
        //     $table->string('ip_address', 45)->nullable();
        //     $table->text('access_agent')->nullable();
        //     $table->longText('payload');
        //     $table->timestamp('created_at')->useCurrent();
        //     $table->integer('last_activity')->index();
        // });
    }

    /**
     * Reverse the migrations.
     */
    public function down(): void
    {
        // Schema::dropIfExists('sessions');
        Schema::dropIfExists('accesses');
    }
};
